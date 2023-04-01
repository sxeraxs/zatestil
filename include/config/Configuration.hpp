//
// Created by aalisher on 3/2/23.
//
#pragma once

#include <string>

#include <util/Singleton.hpp>

namespace ztstl::config {

enum Param { Token, Name };

template <class Config>
class Configuration {
   public:
    Configuration() = default;
    ~Configuration() noexcept = default;
    Configuration(Configuration&&) = delete;
    Configuration(Configuration const&) = delete;
    Configuration& operator=(Configuration&&) = delete;
    Configuration& operator=(Configuration const&) = delete;

   public:
    template <Param param>
    [[nodiscard]] std::string get() const noexcept {
        auto config = Config::instance();
        if constexpr (param == Token) {
            return config->getToken();
        } else if constexpr (param == Name) {
            return config->getName();
        }
    }
};
}// namespace ztstl::config
