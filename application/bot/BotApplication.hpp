//
// Created by aalisher on 3/26/23.
//

#pragma once
#include "tgbot/tgbot.h"

#include <websocket/Message.hpp>
#include <websocket/client/Session.hpp>

#include "config/BotConfiguration.hpp"

#include "../Application.hpp"

namespace ztstl::bot {
using Bot = TgBot::Bot;
using BotMessage = TgBot::Message;
using WebSocketMessage = websocket::Message;

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

    void onMessage(std::shared_ptr<websocket::client::Session> const& session, WebSocketMessage const& message);

   private:
    std::shared_ptr<Bot> m_bot;
    websocket::SslContext m_sslContext;
    config::BotConfiguration::Ptr m_config;
    std::unique_ptr<ThreadPool> m_threadPool {nullptr};
    std::shared_ptr<websocket::client::Session> m_session {nullptr};
};

}// namespace ztstl::bot
