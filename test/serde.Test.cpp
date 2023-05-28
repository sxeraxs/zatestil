//
// Created by aalisher on 3/2/23.
//
#include "serde/serde.hpp"

#include <gtest/gtest.h>

#include <log/log.hpp>

struct Foo {
    int a;
    double b;

    serializable_as(Foo, a, b);
};

template <class T>
bool is_valid_json_serializing(T const& object) {
    using namespace ztstl::log;
    using namespace ztstl::serde;

    auto jsonObject = to_json(object);
    auto resultObject = from_json<T>(jsonObject);

    info("json object: {}", jsonObject);

    return resultObject == object;
}

template <class T>
bool is_valid_binary_serializing(T const& object) {
    using namespace ztstl::log;
    using namespace ztstl::serde;
    auto binaryObject = to_binary(object);
    auto resultObject = from_binary<T>(binaryObject);
    return resultObject == object;
}

TEST(serde, _0) {
    Foo foo {1, 2.5};

    ASSERT_TRUE(is_valid_json_serializing(foo));
    ASSERT_TRUE(is_valid_binary_serializing(foo));
}

struct AggregatedFoo {
    std::string c {};
    Foo foo {};
    serializable_as(AggregatedFoo, c, foo);
};

TEST(serde, _1) {
    AggregatedFoo aggregatedFoo {"foo", {1, 2.2}};

    ASSERT_TRUE(is_valid_json_serializing(aggregatedFoo));
    ASSERT_TRUE(is_valid_binary_serializing(aggregatedFoo));
}

struct InheritedFoo : public Foo {
    std::string c {};
    inherited_serializable_as(InheritedFoo, Foo, c);
};

TEST(serde, _2) {
    InheritedFoo inheritedFoo {{0, 0.0}, "foo"};
    inheritedFoo.a = 1;
    inheritedFoo.b = 2.3;

    ASSERT_TRUE(is_valid_json_serializing(inheritedFoo));
    ASSERT_TRUE(is_valid_binary_serializing(inheritedFoo));
}

struct ArrayFoo {
    std::vector<int> intArray;
    std::vector<bool> boolArray;
    std::vector<std::string> stringArray;

    serializable_as(ArrayFoo, intArray, boolArray, stringArray);
};

TEST(serde, _3) {
    std::vector<int> intArray {1, 45, 2, 0};
    std::vector<bool> boolArray {true, false, false, true};
    std::vector<std::string> stringArray {"a", "bb", "ccc", "dddd", "eeeee"};

    ArrayFoo arrayFoo {intArray, boolArray, stringArray};
    ASSERT_TRUE(is_valid_json_serializing(arrayFoo));
    ASSERT_TRUE(is_valid_binary_serializing(arrayFoo));
}
