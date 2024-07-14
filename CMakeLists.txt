# 14/07/2024 GIAN CEDRICK G. EPILAN

cmake_minimum_required(VERSION 3.17)

project(sdl_pong
    VERSION 1.0.0
    DESCRIPTION "SDL_PONG "
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

set(TARGET_NAME ${PROJECT_NAME})
 
set(SDL_PONG_SOURCE_FILES
    src/main.cpp
)

# -------------------------------------------------
# FIND LIBRARIES
# -------------------------------------------------
# TODO: WHY ARE WE DOING THIS?
set( SDL_STATIC ON CACHE BOOL "" FORCE )
set( SDL_SHARED OFF CACHE BOOL "" FORCE )

add_subdirectory("${CMAKE_SOURCE_DIR}/lib/sdl2")

# -------------------------------------------------
# GENERATE EXECUTABLE
# -------------------------------------------------
add_executable(${TARGET_NAME} ${SDL_PONG_SOURCE_FILES})


# ------------------------------------------
# TARGET PROPERTIES
# ------------------------------------------
target_link_libraries(${TARGET_NAME}
    PUBLIC SDL2main
    PUBLIC SDL2-static
)

set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME}-${CMAKE_BUILD_TYPE}"
)