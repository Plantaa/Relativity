cmake_minimum_required(VERSION 3.22)

project(relativity LANGUAGES C)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

file(GLOB SRCS "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(relativity ${SRCS})

target_include_directories(relativity
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

add_custom_command(TARGET relativity POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:relativity>/assets
)

target_compile_features(relativity PRIVATE c_std_99)

target_link_libraries(relativity PRIVATE raylib)
