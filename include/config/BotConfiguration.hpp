//
// Created by aalisher on 3/27/23.
//

#pragma once
#include "Configuration.hpp"

namespace ztstl::config {
class BotConfiguration : public Configuration<BotConfiguration>, public util::Singleton<BotConfiguration> {
   private:
    friend class Configuration<BotConfiguration>;
    friend class util::Singleton<BotConfiguration>;

   public:
    BotConfiguration(BotConfiguration&&) = delete;
    BotConfiguration(BotConfiguration const&) = delete;
    BotConfiguration& operator=(BotConfiguration&&) = delete;
    BotConfiguration& operator=(BotConfiguration const&) = delete;

    ~BotConfiguration() noexcept = default;

   private:
    BotConfiguration() = default;

   private:
    static std::string getName() noexcept {
        return "ztsl-bot";
    }

    static std::string getToken() noexcept {
        return R"(6011320582:AAG55SmzWEJApUneCqWt9afjOb2XDDVdxbQ)";
    }
};
}// namespace ztstl::config
