cmake_minimum_required(VERSION 3.22)

project(ReinoDeEmma LANGUAGES C)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

file(GLOB SRCS "${CMAKE_SOURCE_DIR}/src/*.c")

add_executable(ReinoDeEmma ${SRCS})

target_include_directories(ReinoDeEmma
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

add_custom_command(TARGET ReinoDeEmma POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:ReinoDeEmma>/assets
)

target_compile_features(ReinoDeEmma PRIVATE c_std_99)

target_link_libraries(ReinoDeEmma PRIVATE raylib)
