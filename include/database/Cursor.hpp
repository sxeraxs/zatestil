//
// Created by aalisher on 4/30/23.
//

#pragma once
#include "ConstCursor.hpp"

namespace ztstl::database {

template <class Value, Id id, Id tableId>
class Cursor : public ConstCursor<Value, id, tableId> {
   private:
    friend class Table<Value, id, tableId>;

   private:
    explicit Cursor(Writer<id>& writer) noexcept : ConstCursor<Value, id, tableId> {writer}, m_writer {writer} {}
    Cursor(Writer<id>& writer, mdbx::cursor_managed&& cursor, MDBX_error_t error = MDBX_SUCCESS) noexcept :
        ConstCursor<Value, id, tableId> {writer, std::move(cursor), error},
        m_writer {writer} {}

   public:

    void setValue(Value const& value) const {
        setRawValue(serde::to_binary(value));
    }

    void setRawValue(std::string_view const& value) {
        if (this->m_end) {
            throw std::runtime_error(fmt::format("bad cursor db {} table {}", to_string(id), to_string(tableId)));
        }

        auto result = this->m_cursor.current(false);
        if (not result) {
            throw std::runtime_error(fmt::format("bad current db {} table {}", to_string(id), to_string(tableId)));
        }

        auto code = this->m_cursor.put(result.key, value);
        if (code != MDBX_SUCCESS) {
            throw std::runtime_error(fmt::format("bad put db {} table {}", to_string(id), to_string(tableId)));
        }
    }

   private:
    Writer<id>& m_writer;
};

}// namespace ztstl::database
