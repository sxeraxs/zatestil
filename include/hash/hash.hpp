//
// Created by aalisher on 5/1/23.
//

#pragma once
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#include "Hash.hpp"
#include "Hasher.hpp"

#define hashable_as(T, ...)                                                           \
   public:                                                                            \
    ztstl::hash::Hash<crypto_hash_sha256_BYTES> hash() const {                        \
        ztstl::hash::Hasher hasher {};                                                \
        BOOST_PP_SEQ_FOR_EACH(_update, other, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__)); \
        return hasher.hash<crypto_hash_sha256_BYTES>();                               \
    }

#define _update(R, T, member) hasher.update(member);

namespace ztstl::hash {
template <class T>
Hash<crypto_hash_sha256_BYTES> hash(T const& value) noexcept {
    return value.hash();
}

inline Hash<crypto_hash_sha256_BYTES> parse(std::string const& s) noexcept {
    return Hash<crypto_hash_sha256_BYTES>::parse(s);
}

inline Hash<crypto_hash_sha256_BYTES> null() noexcept {
    static Hash<crypto_hash_sha256_BYTES> nul {};
    return nul;
}

}// namespace ztstl::hash
