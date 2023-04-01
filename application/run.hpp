//
// Created by aalisher on 3/27/23.
//

#pragma once
#include <config/Configuration.hpp>
#include <log/log.hpp>
#include <util/OnScopeExit.hpp>
#include <util/Result.hpp>
#include <util/stacktrace.hpp>

namespace ztstl {

template <class Application, class Config>
int run(int, char const**) {
    using namespace ztstl;
    using namespace util;

    auto result = Result {Result::Success};
    auto config = Config::instance();

    log::initialize(config->template get<config::Name>());

    std::set_terminate([]() {
        try {
            log::critical("aborted {}", util::current_stacktrace_as_string());
            log::uninitialize();
        } catch (...) {}

        std::abort();
    });

    util::OnScopeExit _ {[result] {
        switch (result) {
            case Result::Success: {
                log::info("shutting down successfully");
                break;
            }
            case Result::Error: {
                log::error("shutting down error - {}", result.message);
                break;
            }
            case Result::Critical: {
                log::critical("shutting down critical error - {}", result.message);
                break;
            }
        }
        log::uninitialize();
    }};

    try {
        log::info("starting as {}", config->template get<config::Name>());

        Context context{};
        Application app {config, context};
        app.run();

    } catch (const std::exception& ex) {
        result.code = Result::Error;
        result.message = fmt::format("{} {}", ex.what(), util::current_exception_stacktrace_as_string());
    } catch (...) {
        result.code = Result::Critical;
        result.message = fmt::format("shutting down with error - unknown {}", util::current_exception_stacktrace_as_string());
    }

    return result;
}
}// namespace ztstl
