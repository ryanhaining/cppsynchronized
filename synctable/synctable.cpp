#include "synctable.h"

// SyncTable

synclock::SyncTable::~SyncTable()
{
    for (auto iter = this->locks_table.begin(),
            end = this->locks_table.end();
            iter != end;
            ++iter){
        delete iter->second; 
    }
}


std::mutex * synclock::SyncTable::get_lock_address(void *addr)
{
    this->table_lock.lock(); // lock the table itself
    if(this->locks_table.find(addr)== this->locks_table.end()){
        // this is a new entry, in the table and a mutex must be
        // created
        this->locks_table[addr] = new std::mutex;
    }
    std::mutex *var_lock = this->locks_table[addr]; // get the variable's lock
    this->table_lock.unlock(); // unlock the table

    return var_lock;
}

// _Table_Locker

synclock::_Table_Locker::_Table_Locker(SyncTable & sync, void * addr)
    : synchronizer(sync), finished(false)
{
        // on construction, lock the contained object
        this->var_lock = this->synchronizer.get_lock_address(addr);
        this->var_lock->lock();
}
synclock::_Table_Locker::~_Table_Locker()
{
    // on destruction, unlock it.  This will occur on for-loop
    // exit and in the event that an exception occurs
    this->var_lock->unlock();
}

namespace synclock{
    SyncTable globalsynctable;
}
