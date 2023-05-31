//
// Created by aalisher on 5/8/23.
//
#include <gtest/gtest.h>

#include <coro/coro.hpp>
#include <message/Message.hpp>

#include "TestApplication.hpp"
#include "TestConfiguration.hpp"
#include "TestRequester.hpp"
#include "TestSession.hpp"

class MessageTest : public ::testing::Test {
   protected:
    using Message = ztstl::message::Message;

   protected:
    MessageTest() noexcept {
        using namespace ztstl;
        m_config = test::TestConfiguration::instance();
        m_application = std::make_unique<test::TestApplication>(m_context);
        m_server = std::make_shared<test::TestSession>(m_context, "server");
        m_client = std::make_shared<test::TestSession>(m_context, "client");
        m_requester = std::make_unique<test::TestRequester>(m_context, m_client, m_config);
    }

    void SetUp() override {
        m_application->run();
        ztstl::test::bind(m_server, m_client);
    }

    void TearDown() override {
        m_application->stop();
    }

   protected:
    boost::asio::io_context m_context;
    ztstl::test::TestConfiguration::Ptr m_config;
    std::shared_ptr<ztstl::test::TestSession> m_server;
    std::shared_ptr<ztstl::test::TestSession> m_client;
    std::unique_ptr<ztstl::test::TestRequester> m_requester;
    std::unique_ptr<ztstl::test::TestApplication> m_application;
};

TEST_F(MessageTest, _0) {
    using namespace ztstl::message;
    auto main = [&]() -> coro::task<size_t> {
        auto request = make_request<Action::Start>();
        auto response = co_await m_requester->request(request);
        ztstl::log::info("response {}", ztstl::serde::to_json(response));
        if (response.status.ok) {
            co_return 1;
        }

        if (response.status.message != "timed out") {
            co_return 2;
        }

        if (response.type != ztstl::message::Type::Response) {
            co_return 3;
        }
        co_return 0;
    };
    ASSERT_EQ(coro::sync_wait(main()), 0);
}

TEST_F(MessageTest, _1) {
    using namespace ztstl::message;
    auto main = [&]() -> coro::task<size_t> {
        m_server->setHandle([&](auto const& rawMessage) {
            auto message = ztstl::serde::from_json<Message>(rawMessage);
            message.type = ztstl::message::Type::Response;
            message.status.ok = true;
            m_server->send(ztstl::serde::to_json(message));
        });

        auto request = make_request<Action::Start>();
        auto response = co_await m_requester->request(request);
        ztstl::log::info("response {}", ztstl::serde::to_json(response));
        if (not response.status.ok) {
            co_return 1;
        }

        if (response.type != ztstl::message::Type::Response) {
            co_return 2;
        }

        if (response.id != request.id) {
            co_return 3;
        }

        if (response.action != request.action) {
            co_return 4;
        }
        co_return 0;
    };
    ASSERT_EQ(coro::sync_wait(main()), 0);
}

TEST_F(MessageTest, _2) {
    using namespace ztstl::message;
    auto main = [&]() -> coro::task<size_t> {
        m_server->setHandle([&](auto const& rawMessage) {
            auto response = make_response(ztstl::serde::from_json<Message>(rawMessage), {true, ""});
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            m_server->send(ztstl::serde::to_json(response));
        });

        auto request = make_request<Action::Start>();
        auto response = co_await m_requester->request(request);
        ztstl::log::info("response {}", ztstl::serde::to_json(response));

        if (response.action != request.action) {
            co_return 1;
        }

        if (response.type != ztstl::message::Type::Response) {
            co_return 2;
        }

        if (response.id != request.id) {
            co_return 3;
        }

        co_return 0;
    };
    ASSERT_EQ(coro::sync_wait(main()), 0);
}

TEST_F(MessageTest, _3) {
    using namespace ztstl::message;
    auto main = [&]() -> coro::task<size_t> {
        m_server->setHandle([&](auto const& rawMessage) {
            auto response = make_response(ztstl::serde::from_json<Message>(rawMessage), {true, ""});
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            m_server->send(ztstl::serde::to_json(response));
        });

        auto request = make_request<Action::Start>();
        auto response = co_await m_requester->request(request);
        ztstl::log::info("response {}", ztstl::serde::to_json(response));

        if (response.action != request.action) {
            co_return 1;
        }

        if (response.type != ztstl::message::Type::Response) {
            co_return 2;
        }

        if (response.id != request.id) {
            co_return 3;
        }

        if (not response.status.ok) {
            co_return 4;
        }

        co_return 0;
    };
    ASSERT_EQ(coro::sync_wait(main()), 0);
}
