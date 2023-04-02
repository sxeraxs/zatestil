//
// Created by aalisher on 4/2/23.
//
#include <util/Result.hpp>

namespace ztstl::util {

Result::operator Code() const noexcept {
    return code;
}

bool Result::isOk() const noexcept {
    return code == Success;
}

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
