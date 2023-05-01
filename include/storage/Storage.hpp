//
// Created by aalisher on 5/1/23.
//

#pragma once

#include <config/ServiceConfiguration.hpp>
#include <database/Database.hpp>

#include "Block.hpp"
#include "log/log.hpp"
#include "util/Singleton.hpp"
namespace ztstl::storage {
using namespace database;
class Storage : log_as(storage), public util::Singleton<Storage> {
   private:
    friend class util::Singleton<Storage>;

    using MainDb = Database<Id::_0>;
    using BlocksTable = Table<Block, Id::_0, Id::_0>;

   private:
    explicit Storage(config::ServiceConfiguration::Ptr config) noexcept;

   public:
    ~Storage() noexcept;

    Storage(Storage&&) = delete;
    Storage(Storage const&) = delete;
    Storage& operator=(Storage&&) = delete;
    Storage& operator=(Storage const&) = delete;

   private:
    std::string m_root {};
    MainDb::Ptr m_main {nullptr};
    std::unique_ptr<BlocksTable> m_blocks {nullptr};
};
}// namespace ztstl::storage
