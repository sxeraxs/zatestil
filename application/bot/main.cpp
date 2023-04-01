#include "BotApplication.hpp"
#include "config/BotConfiguration.hpp"

#include "../run.hpp"

int main(int argc, char const** argv) {
    using ztstl::bot::BotApplication;
    using ztstl::config::BotConfiguration;
    return ztstl::run<BotApplication, BotConfiguration>(argc, argv);
}
