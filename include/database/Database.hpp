//
// Created by aalisher on 4/9/23.
//

#pragma once
#include <filesystem>

#include <log/log.hpp>
#include <util/Singleton.hpp>

#include "Writer.hpp"

#include <mdbx.h++>

namespace mdbx {
class env_managed;
class txn_managed;
}// namespace mdbx

namespace ztstl::database {

template <Id id>
class Database : public util::Singleton<Database<id>> {
    log_as(database);

   private:
    template <class, Id, Id>
    friend class Table;
    friend class Reader<id>;
    friend class Writer<id>;

    friend class util::Singleton<Database<id>>;

   private:
    explicit Database(std::filesystem::path const& path);

   public:
    ~Database();

    Database(Database&&) = delete;
    Database(Database const&) = delete;
    Database& operator=(Database&&) = delete;
    Database& operator=(Database const&) = delete;

   public:
    template <class Value, Id tableId, mdbx::key_mode km = mdbx::key_mode::usual, mdbx::value_mode vm = mdbx::value_mode::single>
    std::unique_ptr<Table<Value, id, tableId>> openTable(std::string const& name) noexcept {
        auto writer = Writer(*this);
        auto newMap = writer.m_txn.create_map(name, km, vm);
        writer.commit();

        return std::unique_ptr<Table<Value, id, tableId>> {new Table<Value, id, tableId> {*this, std::move(newMap), name}};
    }

    Reader<id> makeReader() noexcept;

    Writer<id> makeWriter() noexcept;

   private:
    std::shared_mutex m_mutex;
    std::shared_ptr<mdbx::env_managed> m_env;
};

}// namespace ztstl::database
