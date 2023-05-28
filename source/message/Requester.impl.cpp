//
// Created by aalisher on 5/8/23.
//
#include "Requester.cpp"

#include <config/BotConfiguration.hpp>
#include <websocket/client/Session.hpp>

namespace ztstl::message {
template class Requester<websocket::client::Session, config::BotConfiguration>;
}// namespace ztstl::message
