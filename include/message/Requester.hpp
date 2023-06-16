//
// Created by aalisher on 5/3/23.
//

#pragma once
#include <chrono>
#include <coroutine>
#include <memory>
#include <utility>

#include <boost/asio/deadline_timer.hpp>
#include <boost/thread/synchronized_value.hpp>
#include <config/Configuration.hpp>
#include <coro/coro.hpp>
#include <log/log.hpp>

#include "Message.hpp"

namespace ztstl::message {
using Context = boost::asio::io_context;
template <class Session, class Config>
class Requester : public std::enable_shared_from_this<Requester<Session, Config>> {
    log_as(message);

   public:
    class TimedMessage : public std::enable_shared_from_this<TimedMessage> {
       public:
        class Awaiter {
           public:
            explicit Awaiter(std::shared_ptr<TimedMessage> message);
            ~Awaiter() noexcept;
            [[nodiscard]] bool await_ready() const noexcept;

            bool await_suspend(std::coroutine_handle<> handle);

            Message await_resume() noexcept;

           private:
            std::shared_ptr<TimedMessage> m_message;
        };

       public:
        TimedMessage(Context& context, std::shared_ptr<Session> session, Message message, std::chrono::milliseconds duration, std::shared_ptr<std::map<uint64_t, std::shared_ptr<TimedMessage>>> timedMessages) noexcept;
        ~TimedMessage() noexcept;

        TimedMessage(TimedMessage const&) = delete;
        TimedMessage(TimedMessage&&) noexcept = default;
        TimedMessage& operator=(TimedMessage const&) = delete;
        TimedMessage& operator=(TimedMessage&&) noexcept = default;

       public:
        void set(Message const& message) noexcept;

        Awaiter operator co_await() noexcept;

       private:
        Message m_message;
        Context& m_context;
        std::coroutine_handle<> m_handle;
        std::shared_ptr<Session> m_session;
        boost::asio::deadline_timer m_timer;
        std::chrono::milliseconds m_duration;
        boost::synchronized_value<std::optional<Message>> m_response {};
        std::shared_ptr<std::map<uint64_t, std::shared_ptr<TimedMessage>>> m_timedMessages {};
    };

    friend class TimedMessage;

   public:
    Requester(Context& context, std::shared_ptr<Session> session, Config::Ptr config) noexcept;
    ~Requester() noexcept;
    Requester(Requester const&) = default;
    Requester(Requester&&) noexcept = default;
    Requester& operator=(Requester const&) = default;
    Requester& operator=(Requester&&) noexcept = default;

   public:
    TimedMessage& request(Message const& message);

   private:
    Context& m_context;
    Config::Ptr m_config;
    size_t m_handleId {0};
    std::shared_ptr<Session> m_session;
    std::chrono::milliseconds m_duration {0};
    std::shared_ptr<std::map<uint64_t, std::shared_ptr<TimedMessage>>> m_timedMessages;
};
}// namespace ztstl::message
