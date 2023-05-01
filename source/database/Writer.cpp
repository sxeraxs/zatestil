//
// Created by aalisher on 4/11/23.
//

#include "database/Writer.hpp"

#include "database/Database.hpp"
namespace ztstl::database {

template <Id id>
Writer<id>::Writer(Database<id>& database) : Reader<id> {} {
    this->m_isOpen = true;
    this->m_lock = UniqueLock {database.m_mutex};
    this->m_txn = database.m_env->start_write();
    this->trace("open write operation {}", this->m_id);
}
}// namespace ztstl::database
