//
// Created by aalisher on 4/2/23.
//
#pragma once
#include <string>

#include <util/serde.hpp>

namespace ztstl::websocket {
struct Message {
    uint64_t id {0};
    std::string data {};

    serializable_as(Message, id, data);
};
}// namespace ztstl::websocket
