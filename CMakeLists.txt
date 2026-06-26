cmake_minimum_required(VERSION 3.6)
project(moon3dp C)

set(CMAKE_C_STANDARD 99)


find_package(PkgConfig REQUIRED)
pkg_check_modules(SP REQUIRED libserialport)

add_executable(serialtest main.c
        lib/libprintlink.h
        lib/libprintlink_marlin.c)

target_include_directories(serialtest PRIVATE ${SP_INCLUDE_DIRS})
target_link_libraries(serialtest PRIVATE ${SP_LIBRARIES})
