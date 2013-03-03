#include "synchronizer.hpp"

#include <iostream>
#include <thread>
#include <utility>

#if 0

void count_to(synclock::Synchronizer &syncer, int &counter, int raise_on){
    for(int i=0; i < 10000000; ++i){
        synchronized(syncer, &counter){
            counter++;
        }
    }
}
#endif

void count_to(synclock::Synchronizer & syncer, int & counter){
    for(int i=0; i < 100000; ++i){
        synchronized(syncer, &counter){
            std::cout << "inc" << std::endl;
            int a = counter;
            a = a + 1;
            counter = a;
        }
    }
}

int main(){
    synclock::Synchronizer syncer;

    int i = 0;

    std::thread t1(count_to, std::ref(syncer), std::ref(i));
    std::thread t2(count_to, std::ref(syncer), std::ref(i));

    t1.join();
    t2.join();
    std::cout << i << std::endl;

    return 0;
}
