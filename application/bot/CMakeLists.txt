application(NAME ${PROJECT_NAME}.bot
        TYPE executable
        DEPENDENCIES
        tgbot
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}.log
        ${PROJECT_NAME}.util
        ${PROJECT_NAME}.hash
        ${PROJECT_NAME}.config
        ${PROJECT_NAME}.threads
        ${PROJECT_NAME}.websocket.util
        ${PROJECT_NAME}.websocket.client
        )
