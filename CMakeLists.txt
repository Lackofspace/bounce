cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME bounce)
project(${PROJECT_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(HEADERS
    src/map.h
    src/view.h
    src/player.h
)

set(SOURCES src/main.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res 
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

# Build googletest
add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest" 
    "googletest"
)

# Test target
add_executable(
    test_list
    test/test_list.cpp
)

# Link test target against gtest libraries
target_link_libraries(
    test_list
    gtest gtest_main
    ${CONAN_LIBS}
)

if (MSVC)
    # @see https://github.com/google/googletest/blob/master/googletest/README.md
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        /W4
    )
    target_compile_options(
        test_list PRIVATE
        /W4
    )
else ()
    target_compile_options(
        ${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(
        test_list PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()