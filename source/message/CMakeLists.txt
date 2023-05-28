module(NAME message TYPE static
        DEPENDENCIES
        coro
        spdlog::spdlog
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}.websocket.util
        ${PROJECT_NAME}.websocket.client
        )
