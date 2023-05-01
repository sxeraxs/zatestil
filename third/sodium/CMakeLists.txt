hunter_add_package(libsodium)
find_package(libsodium CONFIG REQUIRED)
add_library(sodium ALIAS libsodium::libsodium)
