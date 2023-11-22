cmake_minimum_required(VERSION 3.25)
project(TextureDecoder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

SET(ZSTD_BUILD_STATIC ON)
add_subdirectory(libs/argparse)
add_subdirectory(libs/lz4/build/cmake)
add_subdirectory(libs/zstd/build/cmake EXCLUDE_FROM_ALL)


set(TextureDecoderSrc
        library/src/textureApi.cpp
        library/src/converters.cpp
        library/src/dds/ddsSupport.cpp
        library/src/png/pngSupport.cpp
        library/src/hdr/hdrSupport.cpp
        library/src/tga/tgaSupport.cpp
        library/src/blockCompression.cpp
        library/src/stb.cpp
        libs/half/half.cpp
        library/src/publicApi.cpp
        library/src/logging.cpp
        library/src/fileAbstraction.cpp
        library/src/pixelFormat.cpp
        library/src/pvr/pvrSupport.cpp
        library/src/decoders/etc/etc1.cpp
        library/src/shared.cpp
        )

add_library(TextureDecoder SHARED ${TextureDecoderSrc} library/src/fileAbstraction.cpp)
target_include_directories(TextureDecoder PUBLIC library/include libs/bcdec libs/stb libs/half  libs/zstd/lib)
target_link_libraries(TextureDecoder PUBLIC libzstd_static lz4_static)

add_library(TextureDecoderStatic STATIC ${TextureDecoderSrc})
target_include_directories(TextureDecoderStatic PUBLIC library/include libs/bcdec libs/stb libs/half libs/zstd/lib)
target_link_libraries(TextureDecoderStatic PUBLIC libzstd_static lz4_static)

add_executable(
        converter
        converter/src/main.cpp
        converter/src/main.cpp
        library/src/png/pngSupport.cpp
        library/include/publicApi.h
)
target_link_libraries(
        converter
        PRIVATE
        TextureDecoderStatic
        argparse
)

add_executable(tester tester/src/main.cpp)
target_link_libraries(tester PRIVATE TextureDecoderStatic)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if (NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include(FetchContent)

    FetchContent_Declare(
            fmt
            GIT_REPOSITORY https://github.com/fmtlib/fmt.git
            GIT_TAG master  # You can specify a specific tag or commit if needed
    )

    FetchContent_MakeAvailable(fmt)

    target_link_libraries(TextureDecoder PRIVATE fmt::fmt)
    target_link_libraries(TextureDecoderStatic PRIVATE fmt::fmt)
    target_link_libraries(tester PRIVATE fmt::fmt)
    target_link_libraries(converter PRIVATE fmt::fmt)

endif ()
enable_testing()

target_link_libraries(tester PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tester WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
