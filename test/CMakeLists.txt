application(NAME ${PROJECT_NAME}.test TYPE executable
        DEPENDENCIES
        GTest::gtest
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}.log
        ${PROJECT_NAME}.util
        ${PROJECT_NAME}.hash
        ${PROJECT_NAME}.database

        INCLUDE_DIRECTORIES
        )
