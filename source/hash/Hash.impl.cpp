//
// Created by aalisher on 5/1/23.
//
#include "Hash.cpp"

#include <sodium.h>

namespace ztstl::hash {
template class Hash<crypto_hash_sha256_BYTES>;
}
