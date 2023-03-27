//
// Created by aalisher on 3/26/23.
//
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <log/log.hpp>

namespace ztstl::log {

    void initialize(std::string const& name) {
        spdlog::init_thread_pool(8192, 1);
        auto logLevel = spdlog::level::level_enum::trace;

        std::vector<spdlog::sink_ptr> sinks {};
        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
        sinks.emplace_back(consoleSink);
        auto logger = std::make_shared<spdlog::async_logger>(name, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
        logger->set_level(logLevel);
        spdlog::set_default_logger(logger);

        spdlog::flush_every(std::chrono::seconds(5));
        spdlog::flush_on(spdlog::level::info);
        spdlog::flush_on(spdlog::level::err);
    }

    void uninitialize() {
        spdlog::default_logger()->flush();
        spdlog::shutdown();
        spdlog::drop_all();
    }

}// namespace ztstl
