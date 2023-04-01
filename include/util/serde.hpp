//
// Created by aalisher on 4/1/23.
//

#pragma once
#include <concepts>

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <yas/serialize.hpp>
#include <yas/std_types.hpp>

#include "type_traits.hpp"

#define comparable_as(T, ...)                                                          \
   public:                                                                             \
    bool operator==(T const& other) const {                                            \
        bool result = true;                                                            \
        BOOST_PP_SEQ_FOR_EACH(_compare, other, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)); \
        return result;                                                                 \
    }                                                                                  \
                                                                                       \
    bool operator!=(T const& other) const {                                            \
        return !operator==(other);                                                     \
    }

#define inherited_comparable_as(T, B, ...)                                             \
   public:                                                                             \
    bool operator==(T const& other) const {                                            \
        bool result = B::operator==(other);                                            \
        auto const& value = static_cast<T const&>(other);                              \
        BOOST_PP_SEQ_FOR_EACH(_compare, value, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)); \
        return result;                                                                 \
    }                                                                                  \
                                                                                       \
    bool operator!=(T const& other) const {                                            \
        return !operator==(other);                                                     \
    }

#define serializable_as(T, ...)                    \
   public:                                         \
    template <class Archive>                       \
    void serialize(Archive& archive) {             \
        archive& YAS_OBJECT(nullptr, __VA_ARGS__); \
    }                                              \
    comparable_as(T, __VA_ARGS__)

#define inherited_serializable_as(T, B, ...)             \
   public:                                               \
    template <class Archive>                             \
    void serialize(Archive& archive) {                   \
        auto& base = yas::base_object<B>(*this);         \
        archive& YAS_OBJECT(nullptr, __VA_ARGS__, base); \
    }                                                    \
    inherited_comparable_as(T, B, __VA_ARGS__)

DEFINE_HAS_MEMBER(has_serialize, serialize);

namespace ztstl::serde {

template <class T>
concept Serializable = has_serialize_v<T>;

template <class T>
std::string to_binary(T const& value) {
    yas::mem_ostream ostream {};
    yas::binary_oarchive<yas::mem_ostream, yas::binary | yas::no_header> oarchive {ostream};
    oarchive(value);
    auto buffer = ostream.get_intrusive_buffer();
    return {buffer.data, buffer.size};
}

template <class T>
T from_binary(std::string_view const& buffer) {
    T value {};
    yas::mem_istream istream {buffer.data(), buffer.size()};
    yas::binary_iarchive<yas::mem_istream, yas::binary | yas::no_header> iarchive {istream};
    iarchive(value);
    return value;
}

template <class T>
std::string to_json(T const& value) {
    yas::mem_ostream ostream;
    yas::save<yas::mem | yas::json | yas::compacted>(ostream, value);
    auto buffer = ostream.get_intrusive_buffer();
    return {buffer.data, buffer.size};
}

template <class T>
T from_json(std::string_view const& buffer) {
    T value {};
    std::istringstream istream {std::string {buffer}};
    yas::std_istream_adapter adapter {istream};
    yas::load<yas::file | yas::json | yas::compacted>(adapter, value);
    return value;
}

namespace _ {
    template <class T>
    inline constexpr bool compare(T const& lv, T const& rv) {
        return lv == rv;
    }

    template <>
    inline constexpr bool compare<float>(float const& lv, float const& rv) {
        return std::abs(lv - rv) < 10e-8;
    }

#define _compare(R, T, member) result &= ztstl::serde::_::compare(member, T.member);

}// namespace _

}// namespace ztstl::serde
