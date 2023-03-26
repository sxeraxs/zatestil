//
// Created by aalisher on 3/26/23.
//

#pragma once
#include <string>

namespace ztstl::util {
struct Result {
    enum Code : uint8_t { Success = 0, Error = 1, Critical = 2 } code;
    std::string message;

    operator Code() const {
        return code;
    }
};

bool operator==(Result const& result, Result::Code code) {
    return result.code == code;
}

bool operator==(Result::Code code, Result const& result) {
    return result.code == code;
}

bool operator!=(Result::Code code, Result const& result) {
    return result.code != code;
}

bool operator!=(Result const& result, Result::Code code) {
    return result.code != code;
}

}// namespace ztstl::util
