//
// Created by aalisher on 3/26/23.
//
#include "Application.hpp"

#include <log/log.hpp>
#include <util/stacktrace.hpp>

namespace ztstl {

Application::Application(config::Configuration::Ptr config) :
    m_config {config},
    m_isRunning {false},
    m_signalSet {m_context, SIGINT, SIGTERM} {
    using namespace config;
    m_signalSet.async_wait([this](auto error, auto signal) {
        log::debug("got signal {} with error {} message {}", signal, error.value(), error.message());

        if (not error) {
            m_isRunning = false;
            m_context.stop();
        }
    });

    m_bot = std::make_unique<Bot>(m_config->get<Token>());
    m_threadPool = std::make_unique<ThreadPool>(m_context);
}

void Application::run() {
    m_isRunning = true;
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
        } catch (...) {
            log::error("unknown error {}", util::current_exception_stacktrace_as_string());
        }
    });
}
}// namespace ztstl
