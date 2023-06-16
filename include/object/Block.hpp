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
    Hash prevHash;
    Hash selfHash;
    uint64_t height;
    Transaction::Ptr transaction;
    Transaction::Type transactionType;

    hashable_as(Block, height, prevHash, transaction);
    serializable_as(Block, height, prevHash, transaction, transactionType);
};
}// namespace ztstl::object
