#include "synctable.h"

#include <tuple>

namespace synclock {

    // SyncTable

    std::mutex& SyncTable::get_lock(void *addr)
    {
        std::lock_guard<std::mutex> lock(this->table_lock); // lock the table

        auto itr = this->locks_table.find(addr);
        if (itr == this->locks_table.end()) {
            // this is a new entry in the table, a mutex must be created
            itr = this->locks_table.emplace(
                    std::piecewise_construct,
                    std::make_tuple(addr),
                    std::tuple<>{}).first;
        }

        return itr->second;
    }


    // Table_Locker

    Table_Locker::Table_Locker(void *addr, SyncTable& sync_table)
        : var_lock_holder(sync_table.get_lock(addr))
    { }

    SyncTable Table_Locker::shared_table{};
}
