#pragma once

#include <functional>

namespace ztsl {

namespace util {

    class OnScopeExit {
       public:
        ~OnScopeExit();
        OnScopeExit(OnScopeExit const&) = default;
        OnScopeExit(OnScopeExit&&) noexcept = default;
        OnScopeExit& operator=(OnScopeExit const&) = default;
        OnScopeExit& operator=(OnScopeExit&&) noexcept = default;

        explicit OnScopeExit(std::function<void()> func);

       private:
        std::function<void()> m_function;
    };

}// namespace util

}// namespace ztsl
