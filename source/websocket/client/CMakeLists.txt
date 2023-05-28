module(NAME websocket.client TYPE static
        DEPENDENCIES
        sfe
        OpenSSL::SSL
        OpenSSL::Crypto
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}.log
        )
