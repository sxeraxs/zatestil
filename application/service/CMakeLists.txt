application(NAME ${PROJECT_NAME}.service
        TYPE executable
        DEPENDENCIES
        tgbot
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}.config
        ${PROJECT_NAME}.log
        ${PROJECT_NAME}.threads
        ${PROJECT_NAME}.util
        ${PROJECT_NAME}.websocket
        ${PROJECT_NAME}.websocket.server
        )
