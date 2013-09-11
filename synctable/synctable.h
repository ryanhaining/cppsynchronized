#ifndef __SYNCHRONIZER__H__
#define __SYNCHRONIZER__H__

#include <unordered_map>
#include <mutex>

namespace synclock{
    class SyncTable{
        private:
            std::unordered_map<void *, std::mutex *> locks_table;
            std::mutex table_lock; 

        public:
            SyncTable(){}
            // I might remove the delete's but it seems like a bad idea
            // to have the same entries in multiple tables.  I can't think
            // of why you would want that on purpose
            SyncTable(const SyncTable &) = delete;
            SyncTable & operator=(const SyncTable &) = delete;
            ~SyncTable();
 
            std::mutex * get_lock_address(void *addr);
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
// this is exception safe since the lock_guard releases the lock on
// destruction

#define tablesynchronized(TABLE, ADDR)  \
for(std::pair<std::lock_guard<std::mutex>, bool> pairwithREALLYlongname( \
            std::piecewise_construct, \
            std::make_tuple(std::ref( \
                    *TABLE.get_lock_address( \
                        static_cast<void *>(ADDR)))), \
            std::make_tuple(false)); \
        !pairwithREALLYlongname.second; \
        pairwithREALLYlongname.second = true)


// synchronized(&var) { critical section }
//
// synchronized blocks construct a lock_guard on entry and destroy it
// on exit.  This results in a locking of var for the body of the block.
// It is also exception safe since destruction occurs when an exception
// causes the block to exit
#define synchronized(ADDR)  \
    for(std::pair<std::lock_guard<std::mutex>, bool> pairwithREALLYlongname( \
                std::piecewise_construct, \
                std::make_tuple(std::ref( \
                        *synclock::globalsynctable.get_lock_address( \
                            static_cast<void *>(ADDR)))), \
                std::make_tuple(false)); \
            !pairwithREALLYlongname.second; \
            pairwithREALLYlongname.second = true)

#endif // __SYNCHRONIZER__H__
