#include "synctable.h"

#include <iostream>
#include <thread>
#include <utility>


void count_to(synclock::SyncTable & syncer, int & counter)
{
    for(int i=0; i < 100000; ++i){
        tablesynchronized(syncer, &counter){
            int a = counter;
            a = a + 1;
            counter = a;
        }
    }
}


void global_count_to(int & counter, int throw_on)
{
    try {
        for(int i=0; i < 100000; ++i){
            synchronized(&counter){
                if (throw_on == i){
                    throw i;
                }
                //elongated increment to allow more interleaving
                int a = counter; 
                a = a + 1;
                counter = a;
            }
        }
    } catch (int i) {
        std::cout << "exception occurred with value: " << i << std::endl;
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

    // global sync (the cool one)
    i = 0;

    std::thread t3(global_count_to, std::ref(i), 50000);
    std::thread t4(global_count_to, std::ref(i), -1);

    t3.join();
    t4.join();
    std::cout << i << std::endl;

    return 0;
}

