module(NAME websocket.server TYPE static
        DEPENDENCIES spdlog::spdlog
        sfe
        OpenSSL::SSL
        OpenSSL::Crypto
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}.log
        )
