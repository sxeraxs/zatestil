//
// Created by aalisher on 5/1/23.
//

#pragma once
#include "Transaction.hpp"
#include "hash/hash.hpp"
#include "serde/serde.hpp"
namespace ztstl::object {
using Hash = hash::Hash<crypto_hash_sha256_BYTES>;

struct Block {
    Hash prev;
    uint64_t height;
    Transaction::Ptr transaction;

    hashable_as(Block, height, prev, transaction);
    serializable_as(Block, height, prev, transaction);
};
}// namespace ztstl::object
