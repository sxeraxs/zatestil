//
// Created by aalisher on 3/26/23.
//

#include <util/OnScopeExit.hpp>

namespace ztsl {

namespace util {

    OnScopeExit::OnScopeExit(std::function<void()> func) : m_function {std::move(func)} {}

    OnScopeExit::~OnScopeExit() {
        m_function();
    }

}// namespace util

}// namespace ztsl
