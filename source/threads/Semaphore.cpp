//
// Created by aalisher on 3/26/23.
//

#include "Semaphore.hpp"
namespace ztstl::util {
Semaphore::Semaphore(std::size_t nThreads) : m_nThreads {nThreads} {}

void Semaphore::wait() noexcept {
    std::unique_lock lock {m_mutex};

    if (--m_nThreads == 0) {
        m_cond.notify_all();
        return;
    }

    m_cond.wait(lock, [this] {
        return m_nThreads == 0;
    });
}
}// namespace ztstl::util
