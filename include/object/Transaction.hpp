//
// Created by aalisher on 6/2/23.
//

#pragma once

#include <cstdint>
#include <memory>

#include <hash/hash.hpp>
#include <serde/serde.hpp>
namespace ztstl::object {

struct Transaction {
    using Ptr = std::shared_ptr<Transaction>;

    enum Type {
        CreateAccount,
        UpdateAccount,
        DeleteAccount,

        CreateBalance,
        UpdateBalance,
        DeleteBalance,
    };

    Type type;
    uint64_t id;
    uint64_t height;

    hashable_as(Transaction, type, id, height);
    serializable_as(Transaction, type, id, height);
};

}// namespace ztstl::object
