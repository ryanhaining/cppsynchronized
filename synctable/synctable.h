// contains

#ifndef __SYNCHRONIZER__H__
#define __SYNCHRONIZER__H__

#include <map>
#include <mutex>

namespace synclock{
    class SyncTable{
        friend class _Table_Locker;
        private:
            std::map<void *, std::mutex *> locks_table;
            std::mutex table_lock; 
            void lock_address(void *addr);
            void unlock_address(void *addr);

        public:
            SyncTable(){}
            // I might remove the delete's but it seems like a bad idea
            // to have the same entries in multiple tables.  I can't think
            // of why you would want that on purpose
            SyncTable(const SyncTable &) = delete;
            SyncTable & operator=(const SyncTable &) = delete;
            ~SyncTable();
 
    };
    
    // This class is only for use by the synchronized/tablesynchronized blocks
    // and should not be used directly.  The name of the class is
    // intentionlly poorly formed.
    class _Table_Locker{
        private:
            SyncTable & synchronizer;
            void * addr;

        public:
            bool finished;
            _Table_Locker(SyncTable & sync, void * addr);
            ~_Table_Locker();
    };

    // global table for use in synchronized blocks
    extern SyncTable globalsynctable;
}

// tablesynchronized(synctable, &var) { critical section }
//
// tablesynchronized block takes in a table and an address, locking the
// address for the body of the block, but only in the given table
// this is provided so that groups of unrelated threads do not result in a
// large, slow, globalsynctable.
// using a value in a local SyncTable will NOT add it to tho global synctable
// this is exception safe since the _Table_Locker releases the lock on
// destruction

#define tablesynchronized(TABLE, ADDR)  \
for(synclock::_Table_Locker _table_locker_obj(TABLE, (void*)(ADDR)); \
        !_table_locker_obj.finished; \
        _table_locker_obj.finished = true)


// synchronized(&var) { critical section }
//
// synchronized blocks construct a _Table_Locker on entry and destroy it
// on exit.  This results in a locking of var for the body of the block.
// It is also exception safe since destructon occurs when an exception
// causes the block to exit
#define synchronized(ADDR)  \
for(synclock::_Table_Locker _table_locker_obj(synclock::globalsynctable, (void*)(ADDR)); \
        !_table_locker_obj.finished; \
        _table_locker_obj.finished = true)

#endif // __SYNCHRONIZER__H__
