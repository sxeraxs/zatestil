//
// Created by aalisher on 3/26/23.
//

#include <util/OnScopeExit.hpp>

namespace ztstl::util {

OnScopeExit::OnScopeExit(std::function<void()> func) : m_function {std::move(func)} {}

OnScopeExit::~OnScopeExit() {
    try {
        m_function();
    } catch (...) {}
}

}// namespace ztstl::util
