//
// Created by aalisher on 4/12/23.
//
#pragma once
#include <shared_mutex>
#include <variant>

#include <log/log.hpp>
#include <util/Result.hpp>

#include "Id.hpp"

#include <mdbx.h++>
namespace ztstl::database {
template <Id>
class Database;

template <class, Id, Id>
class Table;

using SharedLock = std::shared_lock<std::shared_mutex>;
using UniqueLock = std::unique_lock<std::shared_mutex>;

template <Id id>
class Reader {
    log_as(database);

   protected:
    template <class, Id, Id>
    friend class Table;

    friend class Database<id>;

   private:
    explicit Reader(Database<id>& database);

   protected:
    Reader() noexcept;

   public:
    ~Reader() noexcept;
    Reader(Reader const&) = delete;
    Reader(Reader&&) noexcept = default;
    Reader& operator=(Reader const&) = delete;
    Reader& operator=(Reader&&) noexcept = default;

   public:
    [[nodiscard]] util::Result const& result() const noexcept;

    Reader& operator<<(util::Result const& result) noexcept;

   private:
    void abort() noexcept;

    void commit() noexcept;

    void unlock() noexcept;

   protected:
    uint64_t m_id;
    mdbx::txn_managed m_txn;
    std::atomic_bool m_isOpen {true};
    std::variant<SharedLock, UniqueLock> m_lock;
    util::Result m_result {util::Result::Success};
};
}// namespace ztstl::database
