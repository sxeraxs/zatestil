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

inline uint64_t next_id() noexcept {
    static std::atomic_uint64_t id = 0;
    return ++id;
}

template <Action action>
Message make_request() {
    Message request {};
    request.id = next_id();
    request.action = action;
    request.type = Type::Request;
    return request;
}

template <Action action, class Payload, class... Args>
Message make_request(Args... args) {
    auto payload = Payload {std::forward<Args>(args)...};
    Message request {};
    request.id = next_id();
    request.action = action;
    request.type = Type::Request;
    request.data = serde::to_json(payload);
    return request;
}

inline Message make_response(Message const& request, Status status) {
    Message response {};
    response.id = request.id;
    response.type = Type::Response;
    response.action = request.action;
    response.status = std::move(status);
    return response;
}

template <class Payload, class... Args>
Message make_response(Message const& request, Status status, Args... args) {
    auto payload = Payload {std::forward<Args>(args)...};
    Message response {};
    response.id = request.id;
    response.type = Type::Response;
    response.action = request.action;
    response.status = std::move(status);
    response.data = serde::to_json(payload);
    return response;
}

template <Action action, class Payload, class... Args>
Message make_notify(Status status, Args... args) {
    auto payload = Payload {std::forward<Args>(args)...};
    Message notify {};
    notify.id = next_id();
    notify.action = action;
    notify.type = Type::Notify;
    notify.status = std::move(status);
    notify.data = serde::to_json(payload);
    return notify;
}

template <Action action>
Message make_notify(Status status) {
    Message notify {};
    notify.id = next_id();
    notify.action = action;
    notify.type = Type::Notify;
    notify.status = std::move(status);
    return notify;
}

}// namespace ztstl::message
