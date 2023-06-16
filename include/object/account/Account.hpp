//
// Created by aalisher on 5/1/23.
//

#pragma once
#include "serde/serde.hpp"
namespace ztstl::object {

struct Account {
    enum class Status { New, Registered };

    struct Name {
        std::string user;
        std::string last;
        std::string first;

        serializable_as(Name, user, last, first);
    };

    uint64_t id {};
    Status status {Status::New};
    Name name;
    serializable_as(Account, id, status, name);
};
}// namespace ztstl::object
