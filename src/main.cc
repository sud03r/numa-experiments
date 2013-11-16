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

ostream& operator<<(std::ostream& os, const bitmask& bm)
{
    for(size_t i=0;i<bm.size;++i)
        os << numa_bitmask_isbitset(&bm, i);
    return os;
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
    int numcpus = numa_num_task_cpus();
    cout << "numa_available() " << numa_available() << endl;
    numa_set_localalloc();

    // This displays a bitset of cores on each node
    bitmask* bm = numa_bitmask_alloc(numcpus);
    for (int i=0; i <= numa_max_node() ; i++)
    {
        numa_node_to_cpus(i, bm);
        std::cout << "numa node " << i << " " << *bm << " " << numa_node_size(i, 0) << std::endl;
    }
    numa_bitmask_free(bm);
    return 0;
}

