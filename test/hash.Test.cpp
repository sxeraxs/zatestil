//
// Created by aalisher on 5/1/23.
//
#include <gtest/gtest.h>

#include <hash/hash.hpp>
#include <log/log.hpp>

struct HashFoo {
    std::string data;

    hashable_as(HashFoo, data);
};

TEST(hash, _0) {
    using namespace ztstl;

    auto nullhash = hash::null();
    log::info("{}", nullhash.string());

    auto parsednull = hash::parse(nullhash.string());
    ASSERT_EQ(parsednull, nullhash);

    HashFoo foo0 {"some text"};
    auto hash0 = hash::hash(foo0);
    log::info("{}", hash0.string());
    auto newHash0 = hash::parse(hash0.string());

    ASSERT_EQ(hash0, newHash0);

    HashFoo foo1 {"some texv"};
    auto hash1 = hash::hash(foo1);
    log::info("{}", hash1.string());
    ASSERT_NE(hash0, hash1);

    HashFoo foo2 {"some text"};
    auto hash2 = hash::hash(foo2);
    log::info("{}", hash2.string());
    ASSERT_EQ(hash0, hash2);
}
