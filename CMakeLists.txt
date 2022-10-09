cmake_minimum_required(VERSION 3.8)
project(curl-example)

add_definitions("-g")

add_executable(
        http
        ${CMAKE_CURRENT_SOURCE_DIR}/source/http.c
)

target_link_libraries(
        http
        curl
)

add_executable(
        https
        ${CMAKE_CURRENT_SOURCE_DIR}/source/https.c
)

target_link_libraries(
        https
        curl
)

add_executable(
        multi
        ${CMAKE_CURRENT_SOURCE_DIR}/source/multi.c
)

target_link_libraries(
        multi
        curl
)

add_executable(
        post
        ${CMAKE_CURRENT_SOURCE_DIR}/source/post.c
)

target_link_libraries(
        post
        curl
)

add_executable(
        simple
        ${CMAKE_CURRENT_SOURCE_DIR}/source/simple.c
)

target_link_libraries(
        simple
        curl
)

add_executable(
        download
        ${CMAKE_CURRENT_SOURCE_DIR}/source/download.cpp
)

target_link_libraries(
        download
        curl
)
