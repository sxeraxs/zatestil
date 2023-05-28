//
// Created by aalisher on 5/9/23.
//

#pragma once

#include <config/Configuration.hpp>

namespace ztstl::test {
class TestConfiguration : public config::Configuration<TestConfiguration>, public util::Singleton<TestConfiguration> {
   private:
    friend class Configuration<TestConfiguration>;
    friend class util::Singleton<TestConfiguration>;

   private:
    static std::string getRequestTimeout() noexcept {
        return "1000";
    }
};
}// namespace ztstl::test
