#include <numa.h>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <pthread.h>
#include <link.h>
#include <numaif.h>

using namespace std;
using namespace boost::posix_time;
extern int driverMain(char*, int);

void pin_to_core(int core)
{
    // A bitset where each bit represents a cpu
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core, &cpuset);
    // make this thread run on the specified core.
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
}

void threadFunc(int core)
{
    pin_to_core(core);
    ptime t1 = microsec_clock::universal_time();
    
    // Allocate 2 MB of fata on this node
    int memSize = 0;//2 * 1024 * 1024; 
    char* y = NULL;//static_cast<char*> (numa_alloc_local(memSize));

    // Call driverMain which calls the bigLibrary functions and
    // Does something with this data randomly
    driverMain(y, memSize);

    ptime t2 = microsec_clock::universal_time();
    time_duration duration = (t2 - t1); 

    int cpu = sched_getcpu();
    int node = numa_node_of_cpu(cpu);
    printf("Time Elapsed for thread on core %d == %d on Node %d : %ld\n", core, cpu, node, duration.total_microseconds());
}

int libInfo(struct dl_phdr_info* info, size_t size, void* data)
{
    if (strstr(info->dlpi_name, "bigLib.so") != NULL)
    {
        void* libBaseAddress = (void*)info->dlpi_addr;
        printf("name=%s (%d segments) address=%p\n", info->dlpi_name, info->dlpi_phnum, libBaseAddress);
        
        int status[1];
        int ret_code;
        status[0] = -1;
        ret_code = move_pages(0 , 1, &libBaseAddress, NULL, status, 0);
        printf("Memory for library is at Node: %d \n", status[0]);
    }
    return 0;
}

int main(int argc, const char **argv)
{
    // Print Library info.
    dl_iterate_phdr(libInfo, NULL);

    if (numa_available() != 0)
    {
        cout << "NUMA not supported on this machine" << endl;
        exit(1);
    }

    int numCores = numa_num_task_cpus();
    int numSockets = numa_max_node() + 1;
    cout << "NUMA machine with " << numCores << " cores and " << numSockets << " Socket(s)" << endl;

    vector<boost::thread*> tPtrs;
    for (int coreIdx = 0; coreIdx < numSockets ; coreIdx++)
    {
        // 4 sockets, each with 6 cores with core 
        // Ids as : 4*i, 4*i + 1, 4*i + 2, 4*i + 3 where i = 0..5
        // This implies, core 0, 1, 2, 3 will lie on different sockets
        tPtrs.push_back(new boost::thread(threadFunc, coreIdx));
    }

    for (int i = 0; i < tPtrs.size(); i++)
    {
        boost::thread* t = tPtrs.at(i);
        assert(t);
        t->join();
        delete t;
    }
    return 0;
}

