//
// Created by aalisher on 3/26/23.
//

#pragma once
#include <tgbot/tgbot.h>

#include <config/Configuration.hpp>

namespace ztsl {

class Application {
   private:
    using Bot = TgBot::Bot;
    using Message = TgBot::Message;

   public:
    explicit Application(config::Configuration::Ptr config);

    ~Application() noexcept = default;
    Application(Application&&) = delete;
    Application(Application const&) = delete;
    Application& operator=(Application&&) = delete;
    Application& operator=(Application const&) = delete;

   public:
    void run();

   private:
    config::Configuration::Ptr m_config;
    std::unique_ptr<Bot> m_bot;
};

}// namespace ztsl
