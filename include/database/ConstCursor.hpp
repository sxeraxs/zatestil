//
// Created by aalisher on 5/1/23.
//

#pragma once
#include <spdlog/spdlog.h>

#include <util/serde.hpp>

#include "Id.hpp"

#include <mdbx.h++>

namespace ztstl::database {

template <class Value, Id id, Id tableId>
class Table;

template <Id id>
class Reader;

template <Id id>
class Writer;

template <class Value, Id id, Id tableId>
class EndCursor {};

template <class Value, Id id, Id tableId>
class ConstCursor {
   private:
    friend class Table<Value, id, tableId>;

   protected:
    explicit ConstCursor(Reader<id>& reader) noexcept : m_end {true}, m_reader {reader} {}
    ConstCursor(Reader<id>& reader, mdbx::cursor_managed&& cursor, MDBX_error_t error = MDBX_SUCCESS) noexcept :
        m_reader {reader},
        m_cursor {std::move(cursor)} {
        m_end = error != MDBX_SUCCESS or m_cursor.eof();
    };

   public:
    ~ConstCursor() noexcept = default;

   public:
    bool equal(EndCursor<Value, id, tableId> const&) const {
        return m_end;
    }

    bool equal(ConstCursor const& other) const {
        if (m_end or other.m_end) {
            return m_end == other.m_end;
        }

        return key() == other.key() and rawValue() == other.rawValue();
    }

    [[nodiscard]] std::string_view rawValue() const {
        if (m_end) {
            throw std::runtime_error(fmt::format("bad cursor db {} table {}", to_string(id), to_string(tableId)));
        }

        auto result = m_cursor.current(false);
        if (not result) {
            throw std::runtime_error(fmt::format("bad current db {} table {}", to_string(id), to_string(tableId)));
        }

        return result.value.string_view();
    }

    Value value() const {
        return serde::from_binary<Value>(rawValue());
    }

    [[nodiscard]] std::string_view key() const {
        if (m_end) {
            throw std::runtime_error(fmt::format("bad cursor db {} table {}", to_string(id), to_string(tableId)));
        }

        auto result = m_cursor.current(false);
        if (not result) {
            throw std::runtime_error(fmt::format("bad current db {} table {}", to_string(id), to_string(tableId)));
        }

        return result.key.string_view();
    }

    ConstCursor& operator++() {
        if (m_end) {
            throw std::runtime_error(fmt::format("bad cursor db {} table {}", to_string(id), to_string(tableId)));
        }

        auto result = m_cursor.to_next(false);
        m_end = m_cursor.eof();
        if (not result and not m_end) {
            throw std::runtime_error(fmt::format("bad next db {} table {}", to_string(id), to_string(tableId)));
        }

        m_end = m_cursor.eof();
        return *this;
    }

    ConstCursor& operator--() {
        if (m_end) {
            throw std::runtime_error(fmt::format("bad cursor db {} table {}", to_string(id), to_string(tableId)));
        }

        auto result = m_cursor.to_previous(false);
        m_end = m_cursor.eof();
        if (not result and not m_end) {
            throw std::runtime_error(fmt::format("bad prev db {} table {}", to_string(id), to_string(tableId)));
        }

        m_end = m_cursor.eof();
        return *this;
    }

   protected:
    bool m_end {false};
    Reader<id>& m_reader;
    mdbx::cursor_managed m_cursor {};
};
}// namespace ztstl::database
