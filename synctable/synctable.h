#ifndef SYNCHRONIZER__H__
#define SYNCHRONIZER__H__

#include <unordered_map>
#include <mutex>

namespace synclock{
    class SyncTable{
        friend class _Table_Locker;
        private:
            std::unordered_map<void *, std::mutex *> locks_table;
            std::mutex table_lock; 
            std::mutex * get_lock_address(void *addr);

        public:
            SyncTable() = default;
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
    // The lock_guard data member is created and destroyed along with the
    // _Table_Locker object.  The finished data member is used by the for
    // loop to signal completion.
    class _Table_Locker{
        private:
            //used by default by all threads
            static SyncTable shared_table;

            // holds the lock for the lifetime of the _Table_Locker
            std::lock_guard<std::mutex> var_lock_holder;

        public:
            bool finished;
            _Table_Locker(void *addr,
                    SyncTable& sync_table =_Table_Locker::shared_table);
            _Table_Locker(const _Table_Locker &) = delete;
            _Table_Locker & operator=(const _Table_Locker &) = delete;
    };

    // shared table for use in synchronized blocks
}

// the _Table_Lockers have a bunch of capital letters on the end of them
// to (try to) ensure there are no collisions

// tablesynchronized(synctable, &var) { critical section }
//
// tablesynchronized block takes in a table and an address, locking the
// address for the body of the block, but only in the given table
// this is provided so that groups of unrelated threads do not result in a
// large, slow, _Table_locker::shared_table.
// using a value in a local SyncTable will NOT add it to tho shared synctable
#define tablesynchronized(TABLE, ADDR)  \
for(synclock::_Table_Locker _table_locker_obj_ABCDEFAOEUI( \
            static_cast<void *>(ADDR), TABLE); \
        !_table_locker_obj_ABCDEFAOEUI.finished; \
        _table_locker_obj_ABCDEFAOEUI.finished = true)


// synchronized(&var) { critical section }
//
// synchronized blocks construct a _Table_Locker on entry and destroy it
// on exit.  This results in a locking of var for the body of the block.
// It is also exception safe since destructon occurs when an exception
// causes the block to exit, which releases the mutex.
#define synchronized(ADDR)  \
for(synclock::_Table_Locker _table_locker_obj_ABCDEFAOEUI( \
            static_cast<void *>(ADDR)); \
        !_table_locker_obj_ABCDEFAOEUI.finished; \
        _table_locker_obj_ABCDEFAOEUI.finished = true)

#endif // #ifndef SYNCHRONIZER__H__
