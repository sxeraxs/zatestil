//
// Created by aalisher on 3/26/23.
//
#pragma once
#include <condition_variable>
#include <mutex>
namespace ztstl::util {
class Semaphore {
   public:
    explicit Semaphore(std::size_t nThreads);
    ~Semaphore() = default;

    Semaphore(Semaphore&&) = delete;
    Semaphore(Semaphore const&) = delete;
    Semaphore& operator=(Semaphore&&) = delete;
    Semaphore& operator=(Semaphore const&) = delete;

   public:
    void wait() noexcept;

   private:
    std::mutex m_mutex;
    std::size_t m_nThreads;
    std::condition_variable m_cond;
};

}// namespace ztstl::util
