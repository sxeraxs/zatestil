//
// Created by aalisher on 4/30/23.
//

#pragma once

namespace ztstl::database {
enum class Id { _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10 };

inline std::string to_string(Id id) noexcept {
    switch (id) {
        case Id::_0:
            return "0";
        case Id::_1:
            return "1";
        case Id::_2:
            return "2";
        case Id::_3:
            return "3";
        case Id::_4:
            return "4";
        case Id::_5:
            return "5";
        case Id::_6:
            return "6";
        case Id::_7:
            return "7";
        case Id::_8:
            return "8";
        case Id::_9:
            return "9";
        case Id::_10:
            return "10";
    }
    return "0";
}

}// namespace ztstl::database
