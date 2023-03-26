#include <config/Configuration.hpp>
#include <log/log.hpp>
#include <util/OnScopeExit.hpp>

#include "Application.hpp"

int main() {
    using namespace ztsl;
    using namespace config;

    auto config = config::Configuration::instance();

    log::initialize(config->get<Name>());
    util::OnScopeExit _ {[] {
        log::uninitialize();
    }};

    try {
        log::info("starting ...");

        Application app {config};
        app.run();

        log::info("shutting down successfully");
        return 0;
    } catch (const std::exception& ex) {
        log::error("shutting down with error - {}", ex.what());
        return 1;
    } catch (...) {
        log::critical("shutting down with error - unknown");
        return 1;
    }
}
