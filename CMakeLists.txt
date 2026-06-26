cmake_minimum_required(VERSION 4.0)
project(moon3dp C)

set(CMAKE_C_STANDARD 99)


find_package(PkgConfig REQUIRED)
pkg_check_modules(SP REQUIRED libserialport)

add_executable(serialtest main.c
        libmoon3dp.h
        libmoon3dp_marlin.c)

target_include_directories(serialtest PRIVATE ${SP_INCLUDE_DIRS})
target_link_libraries(serialtest PRIVATE ${SP_LIBRARIES})
