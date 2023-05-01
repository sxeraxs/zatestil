module(NAME websocket.util TYPE static
        DEPENDENCIES ${Boost_LIBRARIES} ${PROJECT_NAME}.log sfe)

add_subdirectory(client)
add_subdirectory(server)
