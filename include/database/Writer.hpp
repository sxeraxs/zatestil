//
// Created by aalisher on 4/11/23.
//
#pragma once
#include "Reader.hpp"

namespace ztstl::database {

template <Id id>
class Writer : public Reader<id> {
   private:
    template <class, Id, Id>
    friend class Table;

    friend class Database<id>;

   private:
    explicit Writer(Database<id>& database);

   public:
    ~Writer() noexcept = default;
    Writer(Writer const&) = delete;
    Writer(Writer&&) noexcept = delete;
    Writer& operator=(Writer const&) = delete;
    Writer& operator=(Writer&&) noexcept = delete;
};

}// namespace ztstl::database
