//
// Created by aalisher on 3/26/23.
//
#include "Application.hpp"

#include <log/log.hpp>

namespace ztsl {

Application::Application(config::Configuration::Ptr config) : m_config {config} {
    using namespace config;
    m_bot = std::make_unique<Bot>(m_config->get<Token>());

    m_bot->getEvents().onCommand("start", [&](Message::Ptr const& msg) {
        log::debug("got start command from {} {}", msg->from->id, msg->from->firstName);
        m_bot->getApi().sendMessage(msg->chat->id, fmt::format("Hi, {}!", msg->from->firstName));
    });

    m_bot->getEvents().onAnyMessage([&](TgBot::Message::Ptr const& msg) {
        log::debug("User wrote {}", msg->text);
        if (StringTools::startsWith(msg->text, "/start")) {
            return;
        }
        m_bot->getApi().sendMessage(msg->chat->id, fmt::format("Hey, {}! Your message is {} ", msg->from->firstName, msg->text));
    });
}

void Application::run() {
    try {
        log::debug("Bot username: {}", m_bot->getApi().getMe()->username);
        TgBot::TgLongPoll longPoll {*m_bot};
        while (true) {
            log::debug("Long poll started");
            longPoll.start();
        }
    } catch (TgBot::TgException const& ex) { log::error("{}", ex.what()); }
}
}// namespace ztsl
