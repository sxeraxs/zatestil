//
// Created by aalisher on 3/26/23.
//
#pragma once

#include <boost/asio.hpp>

namespace ztstl::util {

constexpr size_t minNThreads = 4;

class ThreadPool {
   private:
    using Context = boost::asio::io_context;
    using Function = std::function<void()>;

   public:
    explicit ThreadPool(Context& context, size_t nThreads = minNThreads);
    ~ThreadPool() noexcept;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool(ThreadPool const&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool const&) = delete;

   public:
    void post(Function const& func);

   private:
    Context& m_context;
    std::vector<std::thread> m_threads;
};

}// namespace ztstl::util
