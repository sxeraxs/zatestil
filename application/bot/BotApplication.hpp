//
// Created by aalisher on 3/26/23.
//

#pragma once
#include "tgbot/tgbot.h"

#include <boost/asio.hpp>

#include "config/BotConfiguration.hpp"

#include "../Application.hpp"

namespace ztstl::bot {

class BotApplication : public Application<BotApplication> {
   private:
    using Bot = TgBot::Bot;
    using Message = TgBot::Message;
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

   private:
    std::shared_ptr<Bot> m_bot;
    config::BotConfiguration::Ptr m_config;
    std::unique_ptr<ThreadPool> m_threadPool {nullptr};
};

}// namespace ztstl::bot
