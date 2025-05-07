cmake_minimum_required(VERSION 3.28)
project(ProjectT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_NAME main)

# --- Option to force FetchContent ---
option(PROJECTT_FORCE_SFML_FETCHCONTENT "Force using FetchContent for SFML" OFF)

# --- Try to find SFML ---
if(NOT PROJECTT_FORCE_SFML_FETCHCONTENT)
    find_package(SFML 3 COMPONENTS system window graphics audio)
endif()

# --- If not found or forced, use FetchContent ---
if(PROJECTT_FORCE_SFML_FETCHCONTENT OR NOT SFML_FOUND)
    message(STATUS "Fetching SFML 3.0.1 via FetchContent...")

    include(FetchContent)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.1
        GIT_SHALLOW    ON
    )
    FetchContent_MakeAvailable(SFML)
endif()


# --- Check if SFML::graphics target exists ---
if(TARGET SFML::graphics)
    set(SFML_LINK_TARGET SFML::graphics)
else()
    message(FATAL_ERROR "SFML::graphics target not found.")
endif()

# --- Executable ---
add_executable(${EXECUTABLE_NAME} src/main.cpp)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${SFML_LINK_TARGET})

# --- Assets ---
if(EXISTS "${CMAKE_SOURCE_DIR}/arial.ttf")
    configure_file("${CMAKE_SOURCE_DIR}/arial.ttf"
                   "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/arial.ttf"
                   COPYONLY)
    message(STATUS "arial.ttf copied to runtime directory.")
else()
    message(WARNING "arial.ttf not found in project root.")
endif()

# --- Platform Messages ---
if(WIN32 AND DEFINED SFML_SOURCE_DIR AND NOT SFML_STATIC_LIBRARIES)
    message(STATUS "On Windows: Ensure SFML DLLs are available in your PATH.")
endif()

if(APPLE)
    if(DEFINED SFML_SOURCE_DIR)
        message(STATUS "macOS: Building SFML from source via FetchContent.")
    else()
        message(STATUS "macOS: Using SFML found via Homebrew.")
    endif()
endif()
