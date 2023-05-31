//
// Created by aalisher on 5/7/23.
//
#include <message/Requester.hpp>

namespace ztstl::message {
template <class Session, class Config>
Requester<Session, Config>::Requester(Context& context, std::shared_ptr<Session> session, Config::Ptr config) noexcept :
    m_context {context},
    m_config {config},
    m_session {session},
    m_timedMessages {std::make_shared<std::map<uint64_t, std::shared_ptr<TimedMessage>>>()} {
    m_duration = std::chrono::milliseconds(std::stoul(m_config->template get<config::Param::RequestTimeout>()));
    m_handleId = m_session->setHandle([this](auto const& rawMessage) {
        try {
            auto message = serde::from_json<Message>(rawMessage);
            if (message.type != Type::Response) {
                return;
            }
            auto it = m_timedMessages->find(message.id);
            if (it == m_timedMessages->end()) {
                return;
            }
            it->second->set(message);
        } catch (std::exception const& ex) { warning("{}", ex.what()); }
    });
}

template <class Session, class Config>
Requester<Session, Config>::~Requester() noexcept {
    m_session->unsetHandle(m_handleId);
}

template <class Session, class Config>
Requester<Session, Config>::TimedMessage& Requester<Session, Config>::request(Message const& message) {
    auto timedMessage = std::make_shared<TimedMessage>(m_context, m_session, message, m_duration, m_timedMessages);
    auto result = m_timedMessages->emplace(message.id, timedMessage);
    return *result.first->second;
}

template <class Session, class Config>
Requester<Session, Config>::TimedMessage::TimedMessage(Context& context, std::shared_ptr<Session> session, Message message, std::chrono::milliseconds duration, std::shared_ptr<std::map<uint64_t, std::shared_ptr<TimedMessage>>> timedMessages) noexcept :
    m_message {std::move(message)},
    m_context {context},
    m_session {session},
    m_timer {m_context},
    m_duration {duration},
    m_timedMessages {timedMessages} {}

template <class Session, class Config>
Requester<Session, Config>::TimedMessage::~TimedMessage() noexcept {
    boost::system::error_code ec {};
    m_timer.cancel(ec);
    m_response->reset();
}

template <class Session, class Config>
void Requester<Session, Config>::TimedMessage::set(const Message& message) noexcept {
    auto value = m_response.synchronize();
    if (value->has_value()) {
        return;
    }
    m_timedMessages->erase(message.id);
    value->emplace(message);
    boost::system::error_code ec {};
    m_timer.cancel(ec);
    m_handle.resume();
}

template <class Session, class Config>
Requester<Session, Config>::TimedMessage::Awaiter Requester<Session, Config>::TimedMessage::operator co_await() noexcept {
    return Awaiter {Requester<Session, Config>::TimedMessage::shared_from_this()};
}

template <class Session, class Config>
Requester<Session, Config>::TimedMessage::Awaiter::Awaiter(std::shared_ptr<TimedMessage> message) :
    m_message {std::move(message)} {}

template <class Session, class Config>
Requester<Session, Config>::TimedMessage::Awaiter::~Awaiter() noexcept {
    boost::system::error_code ec {};
    m_message->m_timer.cancel(ec);
}

template <class Session, class Config>
bool Requester<Session, Config>::TimedMessage::Awaiter::await_ready() const noexcept {
    return false;
}

template <class Session, class Config>
bool Requester<Session, Config>::TimedMessage::Awaiter::await_suspend(std::coroutine_handle<> handle) {
    m_message->m_handle = handle;
    m_message->m_session->send(serde::to_json(m_message->m_message));
    m_message->m_timer.expires_from_now(boost::posix_time::milliseconds(m_message->m_duration.count()));
    m_message->m_timer.async_wait([msg = m_message](auto const& ec) {
        if (ec == boost::asio::error::operation_aborted) {
            return;
        }

        auto response = make_response(msg->m_message, {false, "timed out"});
        msg->set(response);
    });
    return true;
}

template <class Session, class Config>
Message Requester<Session, Config>::TimedMessage::Awaiter::await_resume() noexcept {
    return m_message->m_response.value().value();
}

}// namespace ztstl::message
