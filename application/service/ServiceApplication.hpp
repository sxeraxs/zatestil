//
// Created by aalisher on 4/2/23.
//
#pragma once

#include <websocket/Server.hpp>

#include "config/ServiceConfiguration.hpp"

#include "../Application.hpp"

namespace ztstl::service {

class ServiceApplication : public Application<ServiceApplication> {
   private:
    friend class Application<ServiceApplication>;

   public:
    explicit ServiceApplication(config::ServiceConfiguration::Ptr config, Context& context);

    ~ServiceApplication() noexcept = default;
    ServiceApplication(ServiceApplication&&) = delete;
    ServiceApplication(ServiceApplication const&) = delete;
    ServiceApplication& operator=(ServiceApplication&&) = delete;
    ServiceApplication& operator=(ServiceApplication const&) = delete;

   private:
    void run_impl();

   private:
    config::ServiceConfiguration::Ptr m_config;
    std::shared_ptr<websocket::Server> m_server;
    std::unique_ptr<ThreadPool> m_threadPool {nullptr};
};
}// namespace ztstl::service
