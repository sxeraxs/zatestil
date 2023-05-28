//
// Created by xerax on 5/27/23.
//

#include "TestApplication.hpp"
namespace ztstl::test {

TestApplication::TestApplication(Context& context) : Application<TestApplication> {context} {
    m_threadPool = std::make_unique<ThreadPool>(m_context);
}

void TestApplication::run_impl() {}

void TestApplication::stop() noexcept {
    m_isRunning = false;
    m_context.stop();
    m_signalSet->cancel();
}
}// namespace ztstl::test
