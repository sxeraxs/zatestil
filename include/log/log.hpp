#pragma once
#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <spdlog/spdlog.h>

namespace ztsl {
namespace log {

    void initialize(std::string const& name);

    template <class... Args>
    void debug(fmt::format_string<Args...> fmt, Args&&... args) {
        SPDLOG_DEBUG(fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    void info(fmt::format_string<Args...> fmt, Args&&... args) {
        SPDLOG_INFO(fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    void warning(fmt::format_string<Args...> fmt, Args&&... args) {
        SPDLOG_WARN(fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    void error(fmt::format_string<Args...> fmt, Args&&... args) {
        SPDLOG_ERROR(fmt, std::forward<Args>(args)...);
    }

    template <class... Args>
    void critical(fmt::format_string<Args...> fmt, Args&&... args) {
        SPDLOG_CRITICAL(fmt, std::forward<Args>(args)...);
    }

    void uninitialize();

}// namespace log
}// namespace ztsl
