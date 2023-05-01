//
// Created by aalisher on 4/12/23.
//

#include "database/Reader.hpp"

#include "database/Database.hpp"

#include "third/mdbx/mdbx/mdbx.h++"

namespace ztstl::database {

std::atomic_uint64_t operation_id = 0;

template <Id id>
Reader<id>::Reader() noexcept : m_isOpen {true} {
    m_id = ++operation_id;
}

template <Id id>
Reader<id>::Reader(Database<id>& database) : Reader {} {
    m_isOpen = true;
    m_lock = SharedLock {database.m_mutex};
    m_txn = database.m_env->start_read();
    trace("open read operation {}", m_id);
}

template <Id id>
Reader<id>::~Reader() noexcept {
    if (m_result.isOk()) {
        commit();
        trace("close operation {} with success", m_id);
    } else {
        error("close operation {} with error {}", m_id, m_result.message);
        abort();
    }
    m_isOpen = false;
}

template <Id id>
void Reader<id>::commit() noexcept {
    if (not m_isOpen) {
        return;
    }
    trace("commit operation {}", m_id);
    m_txn.commit();
    m_isOpen = false;
    unlock();
}

template <Id id>
void Reader<id>::abort() noexcept {
    if (not m_isOpen) {
        return;
    }
    trace("abort operation {}", m_id);
    m_txn.abort();
    unlock();
}

template <Id id>
void Reader<id>::unlock() noexcept {
    if (std::holds_alternative<UniqueLock>(m_lock)) {
        std::get<UniqueLock>(m_lock).unlock();
    } else if (std::holds_alternative<SharedLock>(m_lock)) {
        std::get<SharedLock>(m_lock).unlock();
    }
}

template <Id id>
Reader<id>& Reader<id>::operator<<(util::Result const& result) noexcept {
    m_result = result;
    return *this;
}

template <Id id>
util::Result const& Reader<id>::result() const noexcept {
    return m_result;
}

}// namespace ztstl::database
