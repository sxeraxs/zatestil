//
// Created by aalisher on 3/26/23.
//

#pragma once
#include <tgbot/tgbot.h>

#include <boost/asio.hpp>
#include <config/Configuration.hpp>
#include <util/ThreadPool.hpp>

namespace ztstl {

class Application {
   private:
    using Bot = TgBot::Bot;
    using Message = TgBot::Message;
    using ThreadPool = util::ThreadPool;
    using Context = boost::asio::io_context;
    using SignalSet = boost::asio::signal_set;

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
    Context m_context;
    SignalSet m_signalSet;
    std::unique_ptr<Bot> m_bot;
    std::atomic_bool m_isRunning;
    config::Configuration::Ptr m_config;
    std::unique_ptr<ThreadPool> m_threadPool;
};

}// namespace ztstl
