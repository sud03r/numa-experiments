#include <numa.h>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <pthread.h>

using namespace std;
using namespace boost::posix_time;
extern int driverMain();

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
    
    // Call driverMain which calls the bigLibrary functions.
    driverMain();

    ptime t2 = microsec_clock::universal_time();
    cout << "Time Elapsed for thread on core " << core << ":" << (t2 - t1) << endl;
}

int main(int argc, const char **argv)
{
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

