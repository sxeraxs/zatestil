//
// Created by aalisher on 3/2/23.
//
#pragma once

#include <string>

#include <util/Singleton.hpp>

namespace ztstl::config {

enum Param {
    BotToken,
    Name,
    Endpoint,
    Address,
    Port,
    SslCertificate,
    SslDhParams,
    SslPrivateKey,
    SslPassword,
    DataDir,
    RequestTimeout
};

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
        auto self = static_cast<Config const*>(this);
        if constexpr (param == BotToken) {
            return self->getBotToken();
        } else if constexpr (param == Name) {
            return self->getName();
        } else if constexpr (param == Endpoint) {
            return self->getEndpoint();
        } else if constexpr (param == Port) {
            return self->getPort();
        } else if constexpr (param == Address) {
            return self->getAddress();
        } else if constexpr (param == SslCertificate) {
            return self->getSslCertificate();
        } else if constexpr (param == SslDhParams) {
            return self->getSslDhParams();
        } else if constexpr (param == SslPrivateKey) {
            return self->getSslPrivateKey();
        } else if constexpr (param == SslPassword) {
            return self->getSslPassword();
        } else if constexpr (param == DataDir) {
            return self->getDataDir();
        } else if constexpr (param == RequestTimeout) {
            return self->getRequestTimeout();
        }
    }
};
}// namespace ztstl::config
