module(NAME database TYPE static
        DEPENDENCIES
        mdbx
        spdlog::spdlog
        ${PROJECT_NAME}.log
        ${PROJECT_NAME}.util
        )
