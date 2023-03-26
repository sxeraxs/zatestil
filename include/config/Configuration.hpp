//
// Created by aalisher on 3/2/23.
//
#pragma once

#include <string>

#include <util/Singleton.hpp>

namespace ztstl::config {

enum Param { Token, Name };

class Configuration : public util::Singleton<Configuration> {
   public:
    Configuration(Configuration&&) = delete;
    Configuration(Configuration const&) = delete;
    Configuration& operator=(Configuration&&) = delete;
    Configuration& operator=(Configuration const&) = delete;

    ~Configuration() noexcept = default;

   public:
    Configuration() = default;

    template <Param param>
    [[nodiscard]] std::string get() const noexcept {
        if constexpr (param == Token) {
            return getToken();
        }
        if constexpr (param == Name) {
            return getName();
        }
    }

   private:
    [[nodiscard]] virtual std::string getName() const noexcept {
        return "ztstl";
    };

    [[nodiscard]] virtual std::string getToken() const noexcept {
        return R"(6011320582:AAG55SmzWEJApUneCqWt9afjOb2XDDVdxbQ)";
    };
};
}// namespace ztstl::config
