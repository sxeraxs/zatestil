application(NAME ${PROJECT_NAME}
        TYPE executable
        DEPENDENCIES
        ${PROJECT_NAME}.config ${PROJECT_NAME}.log ${PROJECT_NAME}.util
        ${BOOST_LIBRARIES}
        tgbot
        )
