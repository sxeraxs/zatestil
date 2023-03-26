//
// Created by aalisher on 3/26/23.
//

#pragma once
#if defined(_DEBUG) and defined(__linux__)
#define BOOST_STACKTRACE_USE_ADDR2LINE
#include <string>

#include <sfe/sfe.hpp>
#endif
#include <stdexcept>

namespace ztstl::util {

std::string current_stacktrace_as_string() noexcept;

std::string current_exception_stacktrace_as_string() noexcept;
}// namespace ztstl::util
