cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME bounce)
project(${PROJECT_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(HEADERS
    map.h
    view.h
    )

set(SOURCES main.cpp)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res 
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
