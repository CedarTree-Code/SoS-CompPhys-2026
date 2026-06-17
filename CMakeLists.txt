cmake_minimum_required(VERSION 3.28)
project(CMakeSFML&Matplot++Project LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(matplotplusplus
    GIT_REPOSITORY https://github.com/alandefreitas/matplotplusplus
    GIT_TAG origin/master)
FetchContent_GetProperties(matplotplusplus)

if(NOT matplotplusplus_POPULATED)
    FetchContent_Populate(matplotplusplus)
    add_subdirectory(${matplotplusplus_SOURCE_DIR} ${matplotplusplus_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(main 
    src/main.cpp
    src/misc.cpp
    src/planet.cpp
    src/universe.cpp
    src/quadtree.cpp
)

add_executable(treetest
    src/treetest.cpp
    src/misc.cpp
    src/planet.cpp
    src/universe.cpp
    src/quadtree.cpp
)

target_include_directories(main PRIVATE src)
target_compile_features(main PRIVATE cxx_std_23)
target_link_libraries(main PRIVATE SFML::Graphics)
target_link_libraries(main PRIVATE matplot)

target_include_directories(treetest PRIVATE src)
target_compile_features(treetest PRIVATE cxx_std_23)
target_link_libraries(treetest PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)
