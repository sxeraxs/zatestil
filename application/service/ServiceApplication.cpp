//
// Created by aalisher on 4/2/23.
//

#include "ServiceApplication.hpp"
namespace ztstl::service {

ServiceApplication::ServiceApplication(config::ServiceConfiguration::Ptr config, Context& context) :
    Application<ServiceApplication> {context},
    m_config {config} {
    m_threadPool = std::make_unique<ThreadPool>(m_context);
    m_server = std::make_shared<websocket::Server>(m_context, m_config);
    m_storage = storage::Storage::instance(m_config);
}

void ServiceApplication::run_impl() {
    m_server->run();
}
}// namespace ztstl::service
