//
// Created by aalisher on 5/1/23.
//

#pragma once
#include "hash/hash.hpp"
#include "serde/serde.hpp"
namespace ztstl::object {
using Hash = hash::Hash<crypto_hash_sha256_BYTES>;

struct Block {
    Hash prev;
    hashable_as(Block, prev);
    serializable_as(Block, prev);
};
}// namespace ztstl::data
