//
// Created by aalisher on 3/26/23.
//
#include "threads/ThreadPool.hpp"

#include "Semaphore.hpp"
#include "log/log.hpp"
#include "util/stacktrace.hpp"

namespace ztstl::util {

void run(boost::asio::io_context& context) {
    while (not context.stopped()) {
        try {
            context.run();
        } catch (std::exception const& ex) {
            log::error("[threads] {} {}", ex.what(), current_exception_stacktrace_as_string());
        } catch (...) { log::error("[threads] unknown error {}", current_exception_stacktrace_as_string()); }
    }
}

ThreadPool::ThreadPool(ThreadPool::Context& context, size_t nThreads) : m_context {context} {
    nThreads = std::max(nThreads, static_cast<size_t>(std::thread::hardware_concurrency()));
    trace("starting {} threads", nThreads);

    m_threads.reserve(nThreads);

    auto semaphore = std::make_shared<Semaphore>(nThreads + 1);
    for (size_t i = 0; i < nThreads; ++i) {
        m_threads.emplace_back([this, semaphore]() {
            semaphore->wait();
            run(m_context);
        });
    }
    semaphore->wait();
}

void ThreadPool::post(const ThreadPool::Function& func) {
    m_context.post(func);
}

ThreadPool::~ThreadPool() noexcept {
    for (auto& th : m_threads) {
        if (th.joinable()) {
            th.join();
        }
    }

    trace("shutdown {} threads", m_threads.size());
    m_threads.clear();
}

}// namespace ztstl::util
