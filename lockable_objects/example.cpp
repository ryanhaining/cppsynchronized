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
            name(name) { }
        Person(const Person & other) {
            this->age = other.age;
            this->name = other.name;
        }
        
        void set_name(std::string name){
            this->name = name;
        }
        void show(){
            std::cout << this->age << ' ' << this->name << std::endl;
        }

        ~Person(){
        }

};

// a wrapper for an int.  using a .inc() function to do ++
class Counter{
    private:
        int _count;
    public:
        Counter(): _count(0){}
        void inc(){ this->_count++; }
        int count() { return this->_count; }
};

void count_to(synclock::Lockable<Counter> &counter, int raise_on){
    try {
        for(int i=0; i < 10000000; ++i){
            synchronized(counter){
                if (i == raise_on){
                    throw i; // show exception safety
                }
                counter.inc();
            }
        }
    } catch (int i) {
        std::cout << "exception occurred with value: " << i << std::endl;
    }
}

int main(){
    Person bill(18, "bill");
    synclock::Lockable<Person> lp(2, "jane");
    synchronized(lp){
        lp.show();
    }

    bill.show();
    synclock::Lockable<Person> lockable_bill(bill);
    lockable_bill.set_name("william");
    bill.show(); //this will NOT print william
    
    synclock::Lockable<Counter> lc;

    synchronized(lc){
        synchronized(lp){
            std::cout << "proper nesting" << std::endl;
        }
    }
    std::thread t1(count_to, std::ref(lc), 100000);
    std::thread t2(count_to, std::ref(lc), -1);

    t1.join();
    t2.join();
    std::cout << lc.count() << std::endl;

    return 0;
}
