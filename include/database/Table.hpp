//
// Created by aalisher on 4/10/23.
//

#pragma once

#include <utility>

#include "Cursor.hpp"
#include "Database.hpp"
#include "serde/serde.hpp"

#include <mdbx.h++>
namespace ztstl::database {

template <class Value, Id id, Id tableId>
class Table {
    log_as(database);

   public:
    using Handle = mdbx::map_handle;
    using Reader = ztstl::database::Reader<id>;
    using Writer = ztstl::database::Writer<id>;
    using Database = ztstl::database::Database<id>;
    using Cursor = ztstl::database::Cursor<Value, id, tableId>;
    using EndCursor = ztstl::database::EndCursor<Value, id, tableId>;
    using ConstCursor = ztstl::database::ConstCursor<Value, id, tableId>;

    struct Pair {
        std::string_view key;
        Value value;
    };

    struct RawPair {
        std::string_view key;
        std::string_view value;
    };

   public:
    Table(Database& database, Handle&& handle, std::string name) :
        m_name {std::move(name)},
        m_database {database},
        m_handle {std::make_shared<Handle>(handle)} {
        trace("open table {}", m_name);
    }

    ~Table() noexcept {
        trace("close table {}", m_name);
    };

    Table(Table const&) = delete;
    Table(Table&&) noexcept = delete;
    Table& operator=(Table const&) = delete;
    Table& operator=(Table&&) noexcept = delete;

   public:
    EndCursor end(Reader&) const noexcept {
        return EndCursor {};
    }

    ConstCursor begin(Reader& reader) const noexcept {
        auto cursorManaged = reader.m_txn.open_cursor(*m_handle);
        auto result = cursorManaged.to_first(false);
        return ConstCursor {reader, std::move(cursorManaged), result ? MDBX_SUCCESS : MDBX_RESULT_TRUE};
    }

    EndCursor end(Writer&) const noexcept {
        return EndCursor {};
    }

    Cursor begin(Writer& writer) const noexcept {
        auto cursorManaged = writer.m_txn.open_cursor(*m_handle);
        auto result = cursorManaged.to_first(false);
        return Cursor {writer, std::move(cursorManaged), result ? MDBX_SUCCESS : MDBX_RESULT_TRUE};
    }

    void clear(Writer& writer) noexcept {
        auto beg = begin(writer);
        auto en = end(writer);
        erase(beg, en);
    }

    bool empty(Reader& reader) const noexcept {
        return begin(reader).equal(end(reader));
    };

    void erase(Cursor& cursor) noexcept {
        if (cursor.m_end) {
            return;
        }

        cursor.m_cursor.erase(false);
    }

    template <class EndCursorType>
    void erase(Cursor& begin, EndCursorType const& end) noexcept {
        for (auto& i = const_cast<Cursor&>(begin); not i.equal(end); ++i) {
            erase(i);
        }
    }

    void erase(Writer& writer, std::string_view key) noexcept {
        auto cursor = writer.m_txn.open_cursor(*m_handle);
        auto result = cursor.find(key, false);
        if (not result) {
            return;
        }
        Cursor cur {writer, std::move(cursor)};
        return erase(cur);
    }

    template <class EndCursorType>
    Cursor find(Cursor const& beg, EndCursorType const& end, std::function<bool(Pair const& pair)> const& cond) const noexcept {
        auto error = MDBX_SUCCESS;
        auto& i = const_cast<Cursor&>(beg);
        try {
            for (; not i.equal(end); ++i) {
                if (cond({i.key(), i.value()})) {
                    break;
                }
            }
        } catch (std::exception const& ex) {
            error = MDBX_RESULT_TRUE;
            beg.m_reader.m_result.message = ex.what();
            beg.m_reader.m_result.code = util::Result::Error;
        }

        return Cursor {i.m_writer, std::move(i.m_cursor), error};
    }

    template <class EndCursorType>
    Cursor find(Cursor const& beg, EndCursorType const& end, std::function<bool(RawPair const& rawPair)> const& cond) const noexcept {
        auto error = MDBX_SUCCESS;
        auto& i = const_cast<Cursor&>(beg);
        try {
            for (; not i.equal(end); ++i) {
                if (cond({i.key(), i.rawValue()})) {
                    break;
                }
            }
        } catch (std::exception const& ex) {
            error = MDBX_RESULT_TRUE;
            beg.m_reader.m_result.message = ex.what();
            beg.m_reader.m_result.code = util::Result::Error;
        }

        return Cursor {i.m_writer, std::move(i.m_cursor), error};
    }

    template <class EndCursorType>
    ConstCursor find(ConstCursor const& beg, EndCursorType const& end, std::function<bool(Pair const& pair)> const& cond) const noexcept {
        auto error = MDBX_SUCCESS;
        auto& i = const_cast<ConstCursor&>(beg);
        try {
            for (; not i.equal(end); ++i) {
                if (cond({i.key(), i.value()})) {
                    break;
                }
            }
        } catch (std::exception const& ex) {
            error = MDBX_RESULT_TRUE;
            beg.m_reader.m_result.message = ex.what();
            beg.m_reader.m_result.code = util::Result::Error;
        }

        return ConstCursor {i.m_writer, std::move(i.m_cursor), error};
    }

    template <class EndCursorType>
    ConstCursor find(ConstCursor const& beg, EndCursorType const& end, std::function<bool(RawPair const& rawPair)> const& cond) const noexcept {
        auto error = MDBX_SUCCESS;
        auto& i = const_cast<ConstCursor&>(beg);
        try {
            for (; not i.equal(end); ++i) {
                if (cond({i.key(), i.rawValue()})) {
                    break;
                }
            }
        } catch (std::exception const& ex) {
            error = MDBX_RESULT_TRUE;
            beg.m_reader.m_result.message = ex.what();
            beg.m_reader.m_result.code = util::Result::Error;
        }

        return ConstCursor {i.m_writer, std::move(i.m_cursor), error};
    }

    Cursor lowerBound(Writer& writer, std::string_view key) const noexcept {
        auto cursor = writer.m_txn.open_cursor(*m_handle);
        auto error = cursor.lower_bound(key, false);
        return Cursor {writer, std::move(cursor), error ? MDBX_SUCCESS : MDBX_RESULT_TRUE};
    }

    ConstCursor lowerBound(Reader& reader, std::string_view key) const noexcept {
        auto cursor = reader.m_txn.open_cursor(*m_handle);
        auto error = cursor.lower_bound(key, false);
        return ConstCursor {reader, std::move(cursor), error ? MDBX_SUCCESS : MDBX_RESULT_TRUE};
    }

    Cursor insert(Writer& writer, std::string_view key, Value value) const noexcept {
        return insert(writer, key, serde::to_binary(value));
    }

    Cursor insert(Writer& writer, std::string_view key, std::string_view value) const noexcept {
        auto error = MDBX_SUCCESS;
        mdbx::cursor_managed cursorManaged {};
        try {
            cursorManaged = std::move(writer.m_txn.open_cursor(*m_handle));
            cursorManaged.insert(key, value);
        } catch (std::exception const& ex) {
            error = MDBX_RESULT_TRUE;
            writer.m_result.message = ex.what();
            writer.m_result.code = util::Result::Error;
        }

        return Cursor {writer, std::move(cursorManaged), error};
    }

    Cursor upsert(Writer& writer, std::string_view key, Value value) const noexcept {
        return upsert(writer, key, serde::to_binary(value));
    }

    Cursor upsert(Writer& writer, std::string_view key, std::string_view value) const noexcept {
        auto error = MDBX_SUCCESS;
        mdbx::cursor_managed cursor {};
        try {
            cursor = std::move(writer.m_txn.open_cursor(*m_handle));
            cursor.upsert(key, value);
        } catch (std::exception const& ex) {
            error = MDBX_RESULT_TRUE;
            writer.m_result.message = ex.what();
            writer.m_result.code = util::Result::Error;
        }

        return Cursor {writer, std::move(cursor), error};
    }

    Cursor update(Writer& writer, std::string_view key, Value value) const noexcept {
        return update(writer, key, serde::to_binary(value));
    }

    Cursor update(Writer& writer, std::string_view key, std::string_view value) const noexcept {
        auto error = MDBX_SUCCESS;
        mdbx::cursor_managed cursor {};
        try {
            cursor = std::move(writer.m_txn.open_cursor(*m_handle));
            cursor.update(key, value);
        } catch (std::exception const& ex) {
            error = MDBX_RESULT_TRUE;
            writer.m_result.message = ex.what();
            writer.m_result.code = util::Result::Error;
        }

        return Cursor {writer, std::move(cursor), error};
    }

    ConstCursor get(Reader& reader, std::string_view key) const noexcept {
        auto cursor = reader.m_txn.open_cursor(*m_handle);
        auto result = cursor.find(key, false);
        return ConstCursor {reader, std::move(cursor), result ? MDBX_SUCCESS : MDBX_RESULT_TRUE};
    }

    Cursor get(Writer& writer, std::string_view key) const noexcept {
        auto cursor = writer.m_txn.open_cursor(*m_handle);
        auto result = cursor.find(key, false);
        return Cursor {writer, std::move(cursor), result ? MDBX_SUCCESS : MDBX_RESULT_TRUE};
    }

   private:
    std::string m_name;
    Database& m_database;
    std::shared_ptr<Handle> m_handle;
};
}// namespace ztstl::database
