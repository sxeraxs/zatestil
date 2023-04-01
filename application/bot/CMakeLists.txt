application(NAME ${PROJECT_NAME}.bot
        TYPE executable
        DEPENDENCIES
        ${PROJECT_NAME}.config ${PROJECT_NAME}.log ${PROJECT_NAME}.util  ${Boost_LIBRARIES} tgbot
        )
