#ifndef __LOCKABLE__H__
#define __LOCKABLE__H__

#include <mutex>

namespace synclock{

    template <class Enclosed>
    class Lockable : public Enclosed, public std::mutex{
        public:
            // pass any arguments to Enclosed class constructor
            template <typename... Ts>
            Lockable(Ts... params): Enclosed(params...) {}
            // create Lockable from nonlockable.  the behavior of this
            // completely depends on how the Enclosed class's copy
            // constructor is defined.
            Lockable(const Enclosed & other): Enclosed(other) {}
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
    class _Locker{
        private:
            Contained & lockable;
        public:
            bool finished;
            _Locker(Contained & obj): lockable(obj), finished(false) {
                // on construction, lock the contained object
                this->lockable._lockable_sync_lock();
            }
            ~_Locker() {
                // on destruction, unlock it.  This will occur on for-loop
                // exit and in the event that an exception occurs
                this->lockable._lockable_sync_unlock();
            }
    };
}


// for loop creates a Locker, which locks the LKBLEOBJ given
// the condition checks if the Locker::finished variable is true. initially,
// it is false, so the body of the loop is executed
// the for loop sets the finished variable to true, causing the condition to
// fail and the loop to exit
// this triggers the destruction of the Locker which unlocks the LKBLEOBJ
// since destruction will also occur on exception, this is exception safe.
// once the synchronized block is exited, the LKBLEOBJ is guaranteed to be in
// an unlocked state
#define synchronized(LKBLEOBJ)  \
    for(synclock::_Locker<decltype(LKBLEOBJ)> _locker_obj(LKBLEOBJ); \
        !_locker_obj.finished; \
        _locker_obj.finished = true)


// example usage:
//
// synclock::Lockable<Person> jane;
// synchronized(jane){
//     //critical section
// }

#if 0
// exception-unsafe version
#define synchronized(OBJ) \
    for(int _i_lock_val = OBJ._lockable_sync_lock();\
        _i_lock_val == 0; \
        OBJ._lockable_sync_unlock(), ++_i_lock_val)
#endif

#endif // ifndef __LOCKABLE__H__
