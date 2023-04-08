module(NAME threads TYPE static
        DEPENDENCIES spdlog::spdlog  ${Boost_LIBRARIES} ${PROJECT_NAME}.log sfe
        )
