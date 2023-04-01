//
// Created by aalisher on 3/26/23.
//
#include "BotApplication.hpp"

#include "log/log.hpp"
#include "util/stacktrace.hpp"

namespace ztstl::bot {

BotApplication::BotApplication(config::BotConfiguration::Ptr config, Context& context) :
    Application<BotApplication>{context},
    m_config {config} {
    m_threadPool = std::make_unique<ThreadPool>(m_context);
    m_bot = std::make_shared<Bot>(m_config->get<config::Token>());
}

void BotApplication::run_impl() {
    m_bot->getEvents().onCommand("start", [&](Message::Ptr const& msg) {
        m_threadPool->post([this, msg] {
            log::debug("got start command from {} {}", msg->from->id, msg->from->firstName);
            m_bot->getApi().sendMessage(msg->chat->id, fmt::format("Hi, {}!", msg->from->firstName));
        });
    });

    m_bot->getEvents().onAnyMessage([&](TgBot::Message::Ptr const& msg) {
        m_threadPool->post([this, msg] {
            log::debug("User {} wrote {}", msg->from->username, msg->text);
            if (StringTools::startsWith(msg->text, "/start")) {
                return;
            }
            m_bot->getApi().sendMessage(msg->chat->id, fmt::format("Hey, {}! Your message is {} ", msg->from->firstName, msg->text));
        });
    });

    m_threadPool->post([&] {
        try {
            log::debug("bot username: {}", m_bot->getApi().getMe()->username);
            TgBot::TgLongPoll longPoll {*m_bot};
            while (m_isRunning) {
                longPoll.start();
            }
        } catch (TgBot::TgException const& ex) {
            log::error("{} {}", ex.what(), util::current_exception_stacktrace_as_string());
        } catch (std::exception const& ex) {
            log::error("{} {}", ex.what(), util::current_exception_stacktrace_as_string());
        } catch (...) { log::error("unknown error {}", util::current_exception_stacktrace_as_string()); }
    });
}
}// namespace ztstl::bot
