#ifndef __SYNCHRONIZER__H__
#define __SYNCHRONIZER__H__

#include <map>
#include <mutex>

namespace synclock{

    class Synchronizer{
        private:
            std::map<void *, std::mutex *> locks_table;
            std::mutex table_lock;
            
        public:
            Synchronizer(){}
            
            int lock(void *addr){
                table_lock.lock();
                if(this->locks_table.find(addr)== this->locks_table.end()){
                    // this is a new entry, in the table and a mutex must be
                    // created
                    this->locks_table[addr] = new std::mutex;
                }
                this->locks_table[addr]->lock();
                table_lock.unlock();
                return 0;
            }

            int unlock(void *addr){
                table_lock.lock();
                this->locks_table[addr]->unlock();
                table_lock.unlock();
                return 0;
            }
    };

    class _Table_Locker{
        private:
            Synchronizer & synchronizer;
            void * addr;
        public:
            bool finished;
            _Table_Locker(Synchronizer & sync, void * addr)
                : synchronizer(sync), addr(addr), finished(false) {
                // on construction, lock the contained object
                this->synchronizer.lock(addr);
            }
            ~_Table_Locker() {
                // on destruction, unlock it.  This will occur on for-loop
                // exit and in the event that an exception occurs
                this->synchronizer.unlock(addr);
            }
    };
}

#define synchronized(TABLE, ADDR)  \
for(synclock::_Table_Locker _table_locker_obj(TABLE, (void*)(ADDR)); \
        !_table_locker_obj.finished; \
        _table_locker_obj.finished = true)
#endif // __SYNCHRONIZER__H__
