//
// Created by xerax on 5/27/23.
//

#pragma once

#include <application/Application.hpp>
#include <coro/coro.hpp>
#include <message/Notifier.hpp>
#include <message/Requester.hpp>
#include <message/Responser.hpp>
#include <storage/Storage.hpp>
#include <websocket/Server.hpp>

#include "TestConfiguration.hpp"

namespace ztstl::test {

class TestApplication : public Application<TestApplication> {
   private:
    friend class Application<TestApplication>;

   public:
    explicit TestApplication(Context& context);

    ~TestApplication() noexcept = default;
    TestApplication(TestApplication&&) = delete;
    TestApplication(TestApplication const&) = delete;
    TestApplication& operator=(TestApplication&&) = delete;
    TestApplication& operator=(TestApplication const&) = delete;

   public:
    void stop() noexcept;

   private:
    void run_impl();

   private:
    std::unique_ptr<ThreadPool> m_threadPool {nullptr};
};
}// namespace ztstl::test
