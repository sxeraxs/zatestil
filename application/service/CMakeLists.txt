application(NAME ${PROJECT_NAME}.service
        TYPE executable
        DEPENDENCIES
        tgbot
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}.log
        ${PROJECT_NAME}.util
        ${PROJECT_NAME}.config
        ${PROJECT_NAME}.threads
        ${PROJECT_NAME}.storage
        ${PROJECT_NAME}.database
        ${PROJECT_NAME}.websocket.util
        ${PROJECT_NAME}.websocket.server
        )
