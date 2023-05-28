#include <gtest/gtest.h>

#include <database/Database.hpp>
#include <database/Table.hpp>

#include "serde/serde.hpp"

using namespace ztstl::database;

struct DatabaseFoo {
    std::string name;
    serializable_as(DatabaseFoo, name);
};

using TestDatabase0 = Database<Id::_0>;
using TestDatabase1 = Database<Id::_1>;
using TestDatabase2 = Database<Id::_2>;
using TestDatabase3 = Database<Id::_3>;
using TestDatabase4 = Database<Id::_4>;
using TestDatabase5 = Database<Id::_5>;

using TestDatabaseTable00 = Table<DatabaseFoo, Id::_0, Id::_0>;

TEST(Database, _0) {/// << open databases
    ASSERT_NO_THROW(TestDatabase0::instance("./db0"));
    ASSERT_NO_THROW(TestDatabase1::instance("./db1"));
    ASSERT_NO_THROW(TestDatabase2::instance("./db2"));
    ASSERT_NO_THROW(TestDatabase3::instance("./db3"));
    ASSERT_NO_THROW(TestDatabase4::instance("./db4"));
    ASSERT_NO_THROW(TestDatabase5::instance("./db5"));
}

TEST(Database, _1) {/// << create table
    auto db = TestDatabase0::instance("./db0");
    auto table = db->openTable<DatabaseFoo, Id::_0>("foo");
    auto writer = db->makeWriter();
    table->clear(writer);
    ASSERT_TRUE(table->empty(writer));
}

TEST(Database, _2) {/// << create, insert, erase, get, update, upsert, clear, find, lowerBound
    auto db = Database<Id::_0>::instance("./db0");
    DatabaseFoo foo {"some text"};
    std::string_view fooKey = "foo key";
    auto table = db->openTable<DatabaseFoo, Id::_0>("foo");

    {
        auto writer = db->makeWriter();
        table->clear(writer);
        ASSERT_TRUE(table->empty(writer));
    }

    {
        auto writer = db->makeWriter();
        ASSERT_TRUE(table->empty(writer));

        auto insertCursor = table->insert(writer, fooKey, foo);
        ASSERT_TRUE(writer.result().isOk());
        ASSERT_TRUE(not insertCursor.equal(table->end(writer)));
        ASSERT_EQ(insertCursor.value(), foo);

        ASSERT_FALSE(table->empty(writer));

        auto getCursor = table->get(writer, fooKey);
        ASSERT_TRUE(not getCursor.equal(table->end(writer)));
        ASSERT_TRUE(insertCursor.equal(getCursor));
        ASSERT_EQ(insertCursor.key(), getCursor.key());
        ASSERT_EQ(insertCursor.value(), getCursor.value());
        ASSERT_EQ(insertCursor.rawValue(), getCursor.rawValue());

        auto updateCursor = table->update(writer, "bar", DatabaseFoo {"some fake text"});
        ASSERT_FALSE(writer.result().isOk());
        ASSERT_TRUE(updateCursor.equal(table->end(writer)));
    }

    {
        auto reader = db->makeReader();
        ASSERT_TRUE(table->empty(reader));
    }

    {
        auto writer = db->makeWriter();
        DatabaseFoo newFoo {"some new text"};
        auto upsertCursor = table->upsert(writer, fooKey, newFoo);
        ASSERT_TRUE(writer.result().isOk());
        ASSERT_TRUE(not upsertCursor.equal(table->end(writer)));
        ASSERT_EQ(upsertCursor.value(), newFoo);

        auto getCursor = table->get(writer, fooKey);
        ASSERT_TRUE(not getCursor.equal(table->end(writer)));
        ASSERT_TRUE(upsertCursor.equal(getCursor));
        ASSERT_EQ(upsertCursor.key(), getCursor.key());
        ASSERT_EQ(upsertCursor.value(), getCursor.value());
        ASSERT_EQ(upsertCursor.rawValue(), getCursor.rawValue());
    }

    {
        auto reader = db->makeReader();
        ASSERT_FALSE(table->empty(reader));
    }

    {
        auto writer = db->makeWriter();
        ASSERT_FALSE(table->empty(writer));
        table->erase(writer, "fake foo key");
        ASSERT_FALSE(table->empty(writer));
        table->erase(writer, fooKey);
        ASSERT_TRUE(table->empty(writer));
    }

    {
        auto reader = db->makeReader();
        ASSERT_TRUE(table->empty(reader));
    }

    {
        auto writer = db->makeWriter();
        auto findCursor0 = table->find(table->begin(writer), table->end(writer), [&](TestDatabaseTable00::Pair const&) {
            return true;
        });
        ASSERT_TRUE(findCursor0.equal(table->end(writer)));

        auto insertCursor = table->insert(writer, fooKey, foo);
        ASSERT_TRUE(writer.result().isOk());
        ASSERT_TRUE(not insertCursor.equal(table->end(writer)));
        ASSERT_TRUE(insertCursor.equal(insertCursor));
        ASSERT_EQ(insertCursor.value(), foo);

        auto findCursor1 = table->find(table->begin(writer), table->end(writer), [&](TestDatabaseTable00::Pair const&) {
            return true;
        });

        ASSERT_TRUE(findCursor1.equal(table->begin(writer)));

        auto insertCursor1 = table->insert(writer, "12345", foo);
        ASSERT_TRUE(writer.result().isOk());
        ASSERT_TRUE(insertCursor1.equal(insertCursor1));
        ASSERT_TRUE(not insertCursor1.equal(insertCursor));
        ASSERT_TRUE(not insertCursor1.equal(table->end(writer)));
    }

    {
        auto reader = db->makeReader();
        ASSERT_TRUE(not table->empty(reader));

        auto lowerBoundCursor0 = table->lowerBound(reader, "some fake key");
        ASSERT_TRUE(lowerBoundCursor0.equal(table->end(reader)));

        auto getCursor = table->get(reader, fooKey);
        auto lowerBoundCursor1 = table->lowerBound(reader, fooKey);
        ASSERT_TRUE(lowerBoundCursor1.equal(getCursor));
    }

    {
        auto writer = db->makeWriter();
        table->clear(writer);
    }

    {
        auto reader = db->makeReader();
        ASSERT_TRUE(table->empty(reader));
    }
}
