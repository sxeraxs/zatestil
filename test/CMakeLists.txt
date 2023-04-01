application(NAME ${PROJECT_NAME}.test TYPE executable
        DEPENDENCIES GTest::gtest ${PROJECT_NAME}.log
        INCLUDE_DIRECTORIES
        )
