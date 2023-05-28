//
// Created by aalisher on 5/6/23.
//
#pragma once
#include <string>

#include "serde/serde.hpp"
namespace ztstl::message {
enum class Action { Start, GetAccount };

enum class Type { Request, Response, Notify };

struct Status {
    bool ok;
    std::string message;

    serializable_as(Status, ok, message);
};

struct Message {
    Type type;
    uint64_t id;
    Status status;
    Action action;
    std::string data;

    serializable_as(Message, id, type, action, status, data);
};

template <class Payload>
Message make_request(uint64_t id, Action action, Payload payload) {
    Message message;
    message.id = id;
    message.action = action;
    message.type = Type::Request;
    message.data = serde::to_json(payload);
    return message;
}

template <class Payload>
Message make_response(uint64_t id, Action action, Status status, Payload payload) {
    Message message;
    message.id = id;
    message.action = action;
    message.type = Type::Response;
    message.status = std::move(status);
    message.data = serde::to_json(payload);
    return message;
}

template <class Payload>
Message make_notify(uint64_t id, Action action, Status status, Payload payload) {
    Message message;
    message.id = id;
    message.action = action;
    message.type = Type::Notify;
    message.status = std::move(status);
    message.data = serde::to_json(payload);
    return message;
}

}// namespace ztstl::message
