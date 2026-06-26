cmake_minimum_required(VERSION 3.12)
project(moon3dp C)

set(CMAKE_C_STANDARD 99)


find_package(PkgConfig REQUIRED)
pkg_check_modules(SP REQUIRED libserialport)





add_executable(printlink_host main.c
        lib/libprintlink.h
        lib/libprintlink_marlin.c)

target_include_directories(printlink_host PRIVATE ${SP_INCLUDE_DIRS}  ${uSockets_SOURCE_DIR}/src)
target_link_libraries(printlink_host PRIVATE ${SP_LIBRARIES})

