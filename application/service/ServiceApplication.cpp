//
// Created by aalisher on 4/2/23.
//

#include "ServiceApplication.hpp"
namespace ztstl::service {

ServiceApplication::ServiceApplication(config::ServiceConfiguration::Ptr config, Context& context) :
    Application<ServiceApplication> {context},
    m_config {config} {
    m_threadPool = std::make_unique<ThreadPool>(m_context);
}

void ServiceApplication::run_impl() {}
}// namespace ztstl::service