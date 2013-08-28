#include "synctable.h"

namespace synclock {

    // SyncTable

    SyncTable::~SyncTable()
    {
        for (auto iter = this->locks_table.begin(),
                end = this->locks_table.end();
                iter != end;
                ++iter){
            delete iter->second; 
        }
    }


    std::mutex * SyncTable::get_lock_address(void *addr)
    {
        std::lock_guard<std::mutex> lock(this->table_lock); // lock the table

        auto itr = this->locks_table.find(addr);
        if (itr == this->locks_table.end()) {
            // this is a new entry, in the table and a mutex must be created
            itr = this->locks_table.insert(
                    std::make_pair(addr, new std::mutex)).first;
        }

        std::mutex *var_lock = itr->second; // get the variable's lock

        return var_lock;
    }


    // _Table_Locker

    _Table_Locker::_Table_Locker(SyncTable & sync_table, void * addr)
        : var_lock_holder(*sync_table.get_lock_address(addr)),
        finished(false)
    { }


    _Table_Locker::~_Table_Locker() { }


    // The Global SyncTable for synchronized() blocks
    SyncTable globalsynctable;
}
