cmake_minimum_required(VERSION 3.24)
project(raylib_ttf_aliased)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## Dependencies
# --- Raylib 4.2.0
set(RAYLIB_VERSION 4.2.0)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(raylib URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz)
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

# --- Raylib 4.0.0 VCPKG
#find_package(raylib CONFIG REQUIRED)
#target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${raylib_INCLUDE_DIRS})
#target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${raylib_LIBRARIES})

# Declaring executable
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} raylib)

# C++ hardcode options
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_20)
target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -Wall)

# Copy assets
add_custom_command(
    TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_LIST_DIR}/assets
    $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets
    COMMENT "---- Copying assets"
)
