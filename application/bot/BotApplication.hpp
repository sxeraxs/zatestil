//
// Created by aalisher on 3/26/23.
//

#pragma once
#include "tgbot/tgbot.h"

#include <coro/coro.hpp>
#include <message/Notifier.hpp>
#include <message/Requester.hpp>
#include <message/Responser.hpp>
#include <websocket/client/Session.hpp>

#include "config/BotConfiguration.hpp"

#include "../Application.hpp"

namespace ztstl::bot {
using Bot = TgBot::Bot;
using BotMessage = TgBot::Message;

class BotApplication : public Application<BotApplication> {
   private:
    friend class websocket::client::Session;
    friend class Application<BotApplication>;

   public:
    explicit BotApplication(config::BotConfiguration::Ptr config, Context& context);

    ~BotApplication() noexcept = default;
    BotApplication(BotApplication&&) = delete;
    BotApplication(BotApplication const&) = delete;
    BotApplication& operator=(BotApplication&&) = delete;
    BotApplication& operator=(BotApplication const&) = delete;

   private:
    void run_impl();

    coro::task<> onStart(BotMessage::Ptr const& msg);

   private:
    std::shared_ptr<Bot> m_bot;
    websocket::SslContext m_sslContext;
    config::BotConfiguration::Ptr m_config;
    std::unique_ptr<ThreadPool> m_threadPool {nullptr};
    std::shared_ptr<websocket::client::Session> m_session {nullptr};
    std::shared_ptr<message::Requester<websocket::client::Session, config::BotConfiguration>> m_requester;
};

}// namespace ztstl::bot
