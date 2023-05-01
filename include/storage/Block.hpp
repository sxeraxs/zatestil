//
// Created by aalisher on 5/1/23.
//

#pragma once
#include <hash/hash.hpp>
#include <util/serde.hpp>
namespace ztstl::storage {
struct Block {
    std::string name;
    hashable_as(Block, name);
    serializable_as(Block, name);
};
}// namespace ztstl::storage
