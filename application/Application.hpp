//
// Created by aalisher on 3/27/23.
//

#pragma once

#include <boost/asio.hpp>
#include <log/log.hpp>

#include "util/ThreadPool.hpp"

namespace ztstl {

using ThreadPool = util::ThreadPool;
using Context = boost::asio::io_context;

template <class T>
class Application {
   protected:
    using SignalSet = boost::asio::signal_set;

   public:
    explicit Application(Context& context) :
        m_context{context},
        m_isRunning {false} {
        m_signalSet = std::make_unique<SignalSet>(m_context, SIGINT, SIGTERM);
        m_signalSet->async_wait([this](auto error, auto signal) {
            log::debug("got signal {} with error {} message {}", signal, error.value(), error.message());

            if (not error) {
                m_isRunning = false;
                m_context.stop();
            }
        });
    }

    ~Application() = default;
    Application(Application&&) = delete;
    Application(Application const&) = delete;
    Application& operator=(Application&&) = delete;
    Application& operator=(Application const&) = delete;

   public:
    void run(){
        m_isRunning = true;
        static_cast<T*>(this)->run_impl();
    };

   protected:
    Context& m_context;
    std::atomic_bool m_isRunning {false};
    std::unique_ptr<SignalSet> m_signalSet{nullptr};
};

}// namespace ztstl
