cmake_minimum_required(VERSION 3.15)

project(raylib_test CXX)

set(CMAKE_CXX_STANDARD 17)


set(SOURCE_FILES
    main.cpp
)

add_subdirectory(raylib)

add_executable(main ${SOURCE_FILES})

target_link_libraries(main raylib)

if (APPLE)
    target_link_libraries(main "-framework IOKit")
    target_link_libraries(main "-framework Cocoa")
    target_link_libraries(main "-framework OpenGL")
endif()

