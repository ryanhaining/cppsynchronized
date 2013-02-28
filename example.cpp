#include "lockable.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <utility>

// standard person class with name and age
// default constructor private to show it can be worked around
class Person{
    private:
        int age;
        std::string name;

        Person();

    public:
        Person(int age, std::string name):
            age(age),
            name(name){
            }
        
        void show(){
            std::cout << this->age << ' ' << this->name << std::endl;
        }

        ~Person(){
        }

};

class Counter{
    private:
        int _count;
    public:
        Counter(): _count(0){}
        void inc(){ this->_count++; }
        int count() { return this->_count; }
};

void count_to(mysync::Lockable<Counter> &counter, int raise_on){
    try {
        for(int i=0; i < 10000000; ++i){
            synchronized(counter){
                if (i == raise_on){
                    throw i;
                }
                counter.inc();
            }
        }
    } catch (int i) {
        std::cout << "exception occurred with value: " << i << std::endl;
    }
}

int main(){
    mysync::Lockable<Person> lp(2, "jane");
    synchronized(lp){
        lp.show();
    }
    
    mysync::Lockable<Counter> lc;

    std::thread t1(count_to, std::ref(lc), 100);
    std::thread t2(count_to, std::ref(lc), -1);

    t1.join();
    t2.join();
    std::cout << lc.count() << std::endl;


    return 0;
}
