cmake_minimum_required(VERSION 3.0.0)
project(learnsdl VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file (
  ${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_BINARY_DIR}/config.h
)

include_directories(
    /usr/local/include
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/libs/SDL2_image/include
    ${PROJECT_SOURCE_DIR}/libs/SDL2_ttf/include
)
link_directories(
    /usr/local/lib
    ${PROJECT_BINARY_DIR}
)

add_library(SDL2_image
    SHARED
    IMPORTED)
set_target_properties(SDL2_image 
    PROPERTIES 
    IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/libs/SDL2_image/lib/libSDL2_image-2.0.0.dylib)
add_library(SDL2_ttf
    SHARED
    IMPORTED)
set_target_properties(SDL2_ttf 
    PROPERTIES 
    IMPORTED_LOCATION ${PROJECT_SOURCE_DIR}/libs/SDL2_ttf/lib/libSDL2_ttf-2.0.0.dylib)

add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}
)
target_link_libraries(${PROJECT_NAME} src SDL2 SDL2_image SDL2_ttf)

if(APPLE)
    message("Adding add_custom_command")
    add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND echo "Changing SDL extensions reference path"
    COMMAND install_name_tool -add_rpath ${PROJECT_SOURCE_DIR}/libs/SDL2_image/lib
    -add_rpath ${PROJECT_SOURCE_DIR}/libs/SDL2_ttf/lib
    -change /usr/local/lib/libSDL2_image-2.0.0.dylib @rpath/libSDL2_image-2.0.0.dylib
    -change /usr/local/lib/libSDL2_ttf-2.0.0.dylib @rpath/libSDL2_ttf-2.0.0.dylib ${PROJECT_NAME}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
endif(APPLE)

# Test case 
include(CTest)
enable_testing()

# build package installer start
include (InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE  "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
# build package installer end
