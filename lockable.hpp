#ifndef __LOCKABLE__H__
#define __LOCKABLE__H__

#include <mutex>

namespace synclock{

    // Creating a Lockable<Type> object will result in an object
    // subclassing Type and std::mutex.  This yields and object with
    // the original Type's member functions with the additional .lock(), 
    // .unlock() and .try_lock() member functions.
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
                this->lockable.lock();
            }
            ~_Locker() {
                // on destruction, unlock it.  This will occur on for-loop
                // exit and in the event that an exception occurs
                this->lockable.unlock();
            }
    };
}


// synchronize(object){ critical_section }
// similar to javas synchronized block, the object will be locked at the start
// of the block and unlocked at the end of the block to provide safety
// across threads

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
// Lockable<Person> jane;
// void threaded_function(Lockable<Person> & p){
//     synchronized(p){
//         //critical section
//     }

#endif // ifndef __LOCKABLE__H__
