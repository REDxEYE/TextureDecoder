cmake_minimum_required(VERSION 3.25)
project(TextureDecoder)

set(CMAKE_CXX_STANDARD 20)

set(TextureDecoderSrc
        library/src/textureDecoder.cpp
        library/src/converters.cpp
        library/src/dds/loadDDS.cpp
        library/src/png/loadPNG.cpp
        library/src/hdr/loadHDR.cpp
        library/src/rgb.cpp
        library/src/block_compression.cpp
        library/src/stb.cpp
        libs/half/half.cpp
        library/src/api.cpp
        library/src/logging.cpp)

add_library(TextureDecoder SHARED ${TextureDecoderSrc})
add_library(TextureDecoderStatic STATIC ${TextureDecoderSrc})
target_include_directories(TextureDecoder PUBLIC library/include libs/bcdec libs/stb libs/half)
target_include_directories(TextureDecoderStatic PUBLIC library/include libs/bcdec libs/stb libs/half)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_subdirectory(libs/argparse)

add_executable(
        tester
        tester/src/main.cpp
)

target_link_libraries(
        tester
        GTest::gtest_main
        TextureDecoderStatic
)


include(GoogleTest)
gtest_discover_tests(tester
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})


add_executable(
        converter
        converter/src/main.cpp
        converter/src/main.cpp
        library/src/png/loadPNG.cpp
        library/include/api.h)
target_link_libraries(
        converter
        TextureDecoderStatic
        argparse
)