//
// Created by aalisher on 5/1/23.
//

#pragma once
#include <util/serde.hpp>

namespace ztstl::storage {
struct Block {
    std::string name;
    serializable_as(Block, name);
};
}// namespace ztstl::storage
