//
// Created by aalisher on 5/9/23.
//
#pragma once
#include <string>
#include <utility>

#include <boost/asio.hpp>
#include <log/log.hpp>
namespace ztstl::test {
class TestSession : log_as(session) {
   private:
    friend void bind(std::shared_ptr<TestSession> const&, std::shared_ptr<TestSession> const&) noexcept;

   public:
    using Context = boost::asio::io_context;
    using Handle = std::function<void(std::string const&)>;

   public:
    explicit TestSession(Context& context, std::string name) noexcept :
        m_context {context},
        m_name {std::move(name)} {};
    ~TestSession() noexcept = default;
    TestSession(TestSession const&) = delete;
    TestSession(TestSession&&) noexcept = delete;
    TestSession& operator=(TestSession const&) = delete;
    TestSession& operator=(TestSession&&) noexcept = delete;

   public:
    void send(std::string const& message) {
        trace("[{}] send message {}", m_name, message);
        if (m_send) {
            m_send.value()(message);
        }
    };

    bool unsetHandle(size_t handleId) noexcept {
        trace("[{}] unset handle {}", m_name, handleId);
        return m_handles->erase(handleId) != 0;
    };

    size_t setHandle(Handle const& handle) noexcept {
        static std::atomic_size_t handleId = 0;
        size_t id = handleId++;
        m_handles->insert(std::make_pair(id, handle));
        trace("[{}] set handle {}", m_name, id);
        return id;
    };

   private:
    void onRead(std::string const& message) {
        trace("[{}] read message {}", m_name, message);
        auto const handlers = m_handles.get();
        for (auto const& [id, handler] : handlers) {
            m_context.post([handler, message] {
                handler(message);
            });
        }
    }

    void setSend(Handle const& send) noexcept {
        m_send = send;
    }

   private:
    Context& m_context;
    std::string m_name;
    std::optional<Handle> m_send;
    boost::synchronized_value<std::map<size_t, Handle>> m_handles {};
};

void bind(std::shared_ptr<TestSession> const& session1, std::shared_ptr<TestSession> const& session2) noexcept {
    session1->setSend([session2](auto const& message) {
        session2->onRead(message);
    });

    session2->setSend([session1](auto const& message) {
        session1->onRead(message);
    });
}

}// namespace ztstl::test
