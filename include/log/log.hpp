#pragma once
#ifndef SPDLOG_ACTIVE_LEVEL
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#endif

#include <spdlog/spdlog.h>

#include <algorithm>

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

namespace _ {
    template <size_t N>
    struct constexpr_string {
        constexpr constexpr_string(const char (&str)[N]) {
            std::copy_n(str, N, value);
        }

        char value[N];
    };
}// namespace _

template <_::constexpr_string tag>
class Log {
   protected:
    template <class... Args>
    static void debug(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
        log::debug("[{}] {}", tag.value, fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <class... Args>
    static void trace(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
        log::trace("[{}] {}", tag.value, fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <class... Args>
    static void info(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
        log::info("[{}] {}", tag.value, fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <class... Args>
    static void warning(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
        log::warning("[{}] {}", tag.value, fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <class... Args>
    static void error(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
        log::error("[{}] {}", tag.value, fmt::format(fmt, std::forward<Args>(args)...));
    }

    template <class... Args>
    static void critical(fmt::format_string<Args...> fmt, Args&&... args) noexcept {
        log::critical("[{}] {}", tag.value, fmt::format(fmt, std::forward<Args>(args)...));
    }
};
}// namespace ztstl::log

#define log_as(tag) \
   public           \
    ztstl::log::Log<#tag>
