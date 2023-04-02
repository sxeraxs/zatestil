//
// Created by aalisher on 3/26/23.
//

#pragma once
#include <string>

namespace ztstl::util {
struct Result {
    enum Code : uint8_t { Success = 0, Error = 1, Critical = 2 } code;
    std::string message;

    bool isOk() const noexcept;

    operator Code() const noexcept;
};

bool operator==(Result const& result, Result::Code code);

bool operator==(Result::Code code, Result const& result);

bool operator!=(Result::Code code, Result const& result);

bool operator!=(Result const& result, Result::Code code);

}// namespace ztstl::util
