//
// Created by aalisher on 4/2/23.
//
#include <gtest/gtest.h>

#include <log/log.hpp>

int main(int argc, char** argv) {
    using namespace ztstl;
    log::initialize("ztstl-test");

    std::set_terminate([]() {
        try {
            log::uninitialize();
        } catch (...) {}

        std::abort();
    });

    testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    log::uninitialize();

    return result;
}
