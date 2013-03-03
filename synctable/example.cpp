#include "synctable.h"

#include <iostream>
#include <thread>
#include <utility>

#if 0

void count_to(synclock::SyncTable &syncer, int &counter, int raise_on){
    for(int i=0; i < 10000000; ++i){
        synchronized(syncer, &counter){
            counter++;
        }
    }
}
#endif

void count_to(synclock::SyncTable & syncer, int & counter){
    for(int i=0; i < 100000; ++i){
        tablesynchronized(syncer, &counter){
            int a = counter;
            a = a + 1;
            counter = a;
        }
    }
}

void global_count_to(int & counter){
    for(int i=0; i < 100000; ++i){
        synchronized(&counter){
            int a = counter;
            a = a + 1;
            counter = a;
        }
    }
}


int main(){
    synclock::SyncTable syncer;

    // table sync
    int i = 0;

    std::thread t1(count_to, std::ref(syncer), std::ref(i));
    std::thread t2(count_to, std::ref(syncer), std::ref(i));

    t1.join();
    t2.join();
    std::cout << i << std::endl;

    // global sync
    i = 0;

    std::thread t3(global_count_to, std::ref(i));
    std::thread t4(global_count_to, std::ref(i));

    t3.join();
    t4.join();
    std::cout << i << std::endl;
    return 0;
}
