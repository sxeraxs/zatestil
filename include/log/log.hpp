#pragma once
#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <spdlog/spdlog.h>

#include <algorithm>

#define log_as(tag)                                                                           \
   private:                                                                                   \
    template <class... Args>                                                                  \
    static void debug(fmt::format_string<Args...> fmt, Args&&... args) noexcept {             \
        ztstl::log::debug("[{}] {}", #tag, fmt::format(fmt, std::forward<Args>(args)...));    \
    }                                                                                         \
                                                                                              \
    template <class... Args>                                                                  \
    static void trace(fmt::format_string<Args...> fmt, Args&&... args) noexcept {             \
        ztstl::log::trace("[{}] {}", #tag, fmt::format(fmt, std::forward<Args>(args)...));    \
    }                                                                                         \
                                                                                              \
    template <class... Args>                                                                  \
    static void info(fmt::format_string<Args...> fmt, Args&&... args) noexcept {              \
        ztstl::log::info("[{}] {}", #tag, fmt::format(fmt, std::forward<Args>(args)...));     \
    }                                                                                         \
                                                                                              \
    template <class... Args>                                                                  \
    static void warning(fmt::format_string<Args...> fmt, Args&&... args) noexcept {           \
        ztstl::log::warning("[{}] {}", #tag, fmt::format(fmt, std::forward<Args>(args)...));  \
    }                                                                                         \
                                                                                              \
    template <class... Args>                                                                  \
    static void error(fmt::format_string<Args...> fmt, Args&&... args) noexcept {             \
        ztstl::log::error("[{}] {}", #tag, fmt::format(fmt, std::forward<Args>(args)...));    \
    }                                                                                         \
                                                                                              \
    template <class... Args>                                                                  \
    static void critical(fmt::format_string<Args...> fmt, Args&&... args) noexcept {          \
        ztstl::log::critical("[{}] {}", #tag, fmt::format(fmt, std::forward<Args>(args)...)); \
    }

namespace ztstl::log {

void initialize(std::string const& name);

template <class... Args>
void debug(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
    spdlog::debug(fmt, std::forward<Args>(args)...);
}

template <class... Args>
void trace(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
    spdlog::trace(fmt, std::forward<Args>(args)...);
}

template <class... Args>
void info(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
    spdlog::info(fmt, std::forward<Args>(args)...);
}

template <class... Args>
void warning(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
    spdlog::warn(fmt, std::forward<Args>(args)...);
}

template <class... Args>
void error(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
    spdlog::error(fmt, std::forward<Args>(args)...);
}

template <class... Args>
void critical(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
    spdlog::critical(fmt, std::forward<Args>(args)...);
}

void uninitialize();
}// namespace ztstl::log
