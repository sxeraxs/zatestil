module(NAME websocket.util TYPE static
        DEPENDENCIES
        sfe
        OpenSSL::SSL
        OpenSSL::Crypto
        ${Boost_LIBRARIES}
        ${PROJECT_NAME}.log
        )

add_subdirectory(client)
add_subdirectory(server)
