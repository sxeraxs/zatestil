//
// Created by aalisher on 4/9/23.
//
#include <spdlog/spdlog.h>

#include <websocket/websocket.hpp>

namespace ztstl::websocket {

std::string to_string(Endpoint const& endpoint) noexcept {
    return fmt::format("{}:{}", endpoint.address().to_string(), endpoint.port());
}

}// namespace ztstl::websocket
