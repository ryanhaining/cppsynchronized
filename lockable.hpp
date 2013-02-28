#ifndef __LOCKABLE__H__
#define __LOCKABLE__H__

#include <mutex>

namespace mysync{

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

            int _lockable_sync_unlock(){
                this->std::mutex::unlock();
                return 0;
            }
    };

    // this class should only be used by the synchronized macro
    template <class Contained>
    class Locker{
        private:
            Contained & lockable;
        public:
            bool finished;
            Locker(Contained & obj): lockable(obj), finished(false) {
                // on construction, lock the contained object
                this->lockable._lockable_sync_lock();
            }
            ~Locker() {
                // on destruction, unlock it.  This will occur on for-loop
                // exit and in the event that an exception occurs
                this->lockable._lockable_sync_unlock();
            }
    };
}

#define synchronized(OBJ)  \
    for(mysync::Locker<decltype(OBJ)> _locker_obj(OBJ); \
        !_locker_obj.finished; \
        _locker_obj.finished = true)

#if 0
// exception-unsafe version
#define synchronized(OBJ) \
    for(int _i_lock_val = OBJ._lockable_sync_lock();\
        _i_lock_val == 0; \
        OBJ._lockable_sync_unlock(), ++_i_lock_val)
#endif

#endif // ifndef __LOCKABLE__H__
