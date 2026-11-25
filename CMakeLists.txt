cmake_minimum_required(VERSION 3.30)
project(Lodestone.CLI)

set(CMAKE_CXX_STANDARD 23)

set(GLOB_PATHS
        "include/*.h"
        "include/*.hpp"
        "src/*.cpp"
        "src/*.c"
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
file(GLOB_RECURSE FILES CONFIGURE_DEPENDS ${GLOB_PATHS})

# TODO: Use versioned tags instead of main
if (NOT TARGET Lodestone.Core)
    FetchContent_Declare(
            libLodestone
            GIT_REPOSITORY https://github.com/Team-Lodestone/libLodestone.git
            GIT_TAG main
            GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(libLodestone)
endif ()

add_executable(Lodestone.CLI ${FILES})

target_include_directories(Lodestone.CLI
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(Lodestone.CLI PUBLIC
        Lodestone.Level
        Lodestone.Common
        Lodestone.Core
        Lodestone.Conversion
        nbt++
        nlohmann_json
        bio-cpp20
)

setupLinter(Lodestone.CLI)