application(NAME ${PROJECT_NAME}.service
        TYPE executable
        DEPENDENCIES
        ${PROJECT_NAME}.config
        ${PROJECT_NAME}.log
        ${PROJECT_NAME}.util
        ${PROJECT_NAME}.websocket.server
        ${Boost_LIBRARIES} tgbot
        )
