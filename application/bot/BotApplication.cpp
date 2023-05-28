//
// Created by aalisher on 3/26/23.
//
#include "BotApplication.hpp"

#include <object/Account.hpp>
#include <websocket/websocket.hpp>

#include "log/log.hpp"
#include "util/stacktrace.hpp"
namespace ztstl::bot {

uint64_t message_id = 1;

BotApplication::BotApplication(config::BotConfiguration::Ptr config, Context& context) :
    Application<BotApplication> {context},
    m_sslContext {websocket::ssl::context::tls_client},
    m_config {config} {
    m_threadPool = std::make_unique<ThreadPool>(m_context);
    m_bot = std::make_shared<Bot>(m_config->get<config::BotToken>());
    m_requester = std::make_shared<message::Requester<websocket::client::Session, config::BotConfiguration>>(context, m_session, m_config);
}

void BotApplication::run_impl() {
    m_bot->getEvents().onCommand("start", [&](BotMessage::Ptr const& msg) {
        m_threadPool->post([this, msg] {
            auto task = onStart(msg);
        });
    });

    m_bot->getEvents().onAnyMessage([&](TgBot::Message::Ptr const& msg) {
        m_threadPool->post([this, msg] {
            debug("User {} wrote {}", msg->from->username, msg->text);
        });
    });

    m_threadPool->post([&] {
        try {
            debug("bot username: {}", m_bot->getApi().getMe()->username);
            TgBot::TgLongPoll longPoll {*m_bot};
            while (m_isRunning) {
                longPoll.start();
            }
        } catch (TgBot::TgException const& ex) {
            error("{} {}", ex.what(), util::current_exception_stacktrace_as_string());
        } catch (std::exception const& ex) {
            error("{} {}", ex.what(), util::current_exception_stacktrace_as_string());
        } catch (...) { error("unknown error {}", util::current_exception_stacktrace_as_string()); }
    });

    boost::beast::error_code ec {};
    auto cert = m_config->get<config::SslCertificate>();
    m_sslContext.add_certificate_authority(boost::asio::buffer(cert.data(), cert.size()), ec);
    if (ec) {
        error("load certificate {}", ec.what());
        return;
    }

    auto endpoint =
        websocket::Endpoint(boost::asio::ip::address::from_string(m_config->get<config::Address>()), std::stoul(m_config->get<config::Port>()));
    m_session = std::make_shared<websocket::client::Session>(this, endpoint, m_context, m_sslContext);

    auto res = m_session->open();
    if (not res.isOk()) {
        error("{}", res.message);
    }
}

coro::task<> BotApplication::onStart(const BotMessage::Ptr& msg) {
    debug("got start command from {} {}", msg->from->id, msg->from->firstName);
    message::Message message {};
    message.id = message_id++;
    message.action = message::Action::Start;
    message.type = message::Type::Request;
    auto response = co_await m_requester->request(message);
    co_return;
}

}// namespace ztstl::bot
