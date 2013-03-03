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


void synclock::SyncTable::lock_address(void *addr)
{
    this->table_lock.lock(); // lock the table itself
    if(this->locks_table.find(addr)== this->locks_table.end()){
        // this is a new entry, in the table and a mutex must be
        // created
        this->locks_table[addr] = new std::mutex;
    }
    while(!this->locks_table[addr]->try_lock()){
        // try to lock the object, if it cannot be locked,
        // unlock the table, then try to reacquire both locks
        // TODO add sleep?
        this->table_lock.unlock();
        this->table_lock.lock();
    }
    this->table_lock.unlock(); // unlock the table
}

void synclock::SyncTable::unlock_address(void *addr)
{
    this->table_lock.lock();
    this->locks_table[addr]->unlock();
    this->table_lock.unlock();
}

// _Table_Locker

synclock::_Table_Locker::_Table_Locker(SyncTable & sync, void * addr)
    : synchronizer(sync), addr(addr), finished(false)
{
        // on construction, lock the contained object
        this->synchronizer.lock_address(addr);
}
synclock::_Table_Locker::~_Table_Locker()
{
    // on destruction, unlock it.  This will occur on for-loop
    // exit and in the event that an exception occurs
    this->synchronizer.unlock_address(addr);
}


namespace synclock{
    SyncTable globalsynctable;
}
