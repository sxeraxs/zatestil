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
    auto main = [&]() -> coro::task<bool> {
        Message message {};
        message.id = 0;
        message.status.ok = true;
        message.type = ztstl::message::Type::Request;
        message.action = ztstl::message::Action::Start;
        message.data = "Start";

        auto response = co_await m_requester->request(message);
        ztstl::log::info("response {}", ztstl::serde::to_json(response));
        if (response.status.ok) {
            co_return false;
        }

        if (response.status.message != "timed out") {
            co_return false;
        }

        if (response.type != ztstl::message::Type::Response) {
            co_return false;
        }
        co_return true;
    };
    ASSERT_TRUE(coro::sync_wait(main()));
}

TEST_F(MessageTest, _1) {
    auto main = [&]() -> coro::task<bool> {
        m_server->setHandle([&](auto const& rawMessage) {
            auto message = ztstl::serde::from_json<Message>(rawMessage);
            message.type = ztstl::message::Type::Response;
            m_server->send(ztstl::serde::to_json(message));
        });

        Message message {};
        message.id = 0;
        message.status.ok = true;
        message.type = ztstl::message::Type::Request;
        message.action = ztstl::message::Action::Start;
        message.data = "Start";

        auto response = co_await m_requester->request(message);
        ztstl::log::info("response {}", ztstl::serde::to_json(response));
        if (not response.status.ok) {
            co_return false;
        }

        if (response.type != ztstl::message::Type::Response) {
            co_return false;
        }
        co_return true;
    };
    ASSERT_TRUE(coro::sync_wait(main()));
}

TEST_F(MessageTest, _2) {
    auto main = [&]() -> coro::task<bool> {
        m_server->setHandle([&](auto const& rawMessage) {
            auto message = ztstl::serde::from_json<Message>(rawMessage);
            message.type = ztstl::message::Type::Response;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            m_server->send(ztstl::serde::to_json(message));
        });

        Message message {};
        message.id = 0;
        message.status.ok = true;
        message.type = ztstl::message::Type::Request;
        message.action = ztstl::message::Action::Start;
        message.data = "Start";

        auto response = co_await m_requester->request(message);
        ztstl::log::info("response {}", ztstl::serde::to_json(response));

        if (response.type != ztstl::message::Type::Response) {
            co_return false;
        }
        co_return true;
    };
    ASSERT_TRUE(coro::sync_wait(main()));
}
