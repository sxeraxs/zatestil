//
// Created by aalisher on 3/26/23.
//
#include <util/stacktrace.hpp>
namespace ztstl::util {

std::string current_stacktrace_as_string() noexcept {
#if defined(_DEBUG) and defined(__linux__)
    boost::stacktrace::stacktrace bt {};
    return boost::stacktrace::detail::to_string(&bt.as_vector()[0], bt.size());
#else
    return {};
#endif
}

std::string current_exception_stacktrace_as_string() noexcept {
#if defined(_DEBUG) && defined(__linux__)
    auto bt = sfe::get_current_exception_stacktrace();
    return boost::stacktrace::detail::to_string(&bt.as_vector()[0], bt.size());
#else
    return {};
#endif
}

}// namespace ztstl::util
