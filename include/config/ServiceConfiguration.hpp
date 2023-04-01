//
// Created by aalisher on 4/2/23.
//
#pragma once
#include "Configuration.hpp"

namespace ztstl::config {
using namespace ztstl::util;

class ServiceConfiguration : public Configuration<ServiceConfiguration>, public util::Singleton<ServiceConfiguration> {
   private:
    friend class Configuration<ServiceConfiguration>;
    friend class util::Singleton<ServiceConfiguration>;

   public:
    ServiceConfiguration(ServiceConfiguration&&) = delete;
    ServiceConfiguration(ServiceConfiguration const&) = delete;
    ServiceConfiguration& operator=(ServiceConfiguration&&) = delete;
    ServiceConfiguration& operator=(ServiceConfiguration const&) = delete;

    ~ServiceConfiguration() noexcept = default;

   private:
    ServiceConfiguration() = default;

   private:
    static std::string getName() noexcept {
        return "ztsl-service";
    }
};
}// namespace ztstl::config
