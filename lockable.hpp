#ifndef __LOCKABLE__H__
#define __LOCKABLE__H__

#include <mutex>

namespace sync{

    template <class Parent>
    class Lockable : public Parent, public std::mutex{
        public:
            // pass any arguments to Parent class constructor
            template <typename... Ts>
            Lockable(Ts... params): Parent(params...) {}
            ~Lockable(){}

            // these functions should only be used by the synchronized
            // macro.  Use the normal lock() and unlock() functions for
            // your own purposes outside the synchronized block.
            // Their behavior/return types are not guaranteed
            int _lockable_sync_lock(){
                this->std::mutex::lock();
                return 0;
            }

            void _lockable_sync_unlock(){
                this->std::mutex::unlock();
            }
    };
}

#define synchronized(OBJ)  for(int i = OBJ._lockable_sync_lock(); i == 0; \
        OBJ._lockable_sync_unlock(), ++i)


#endif // ifndef __LOCKABLE__H__
