cmake_minimum_required(VERSION 3.28)
project(ProjectT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_NAME main)

# --- SFML Handling ---
# Try to find SFML 3.0.x using find_package
# Let's try "graphics" for SFML 3.0 again.
find_package(SFML 3.0 COMPONENTS graphics QUIET) # << CHANGED BACK TO "graphics"

if(NOT SFML_FOUND)
    message(STATUS "SFML 3.0.x (with component 'graphics') not found via find_package. Attempting FetchContent for SFML 3.0.1.")
    include(FetchContent)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.1
        GIT_SHALLOW    ON
    )
    FetchContent_MakeAvailable(SFML)
    set(SFML_LINK_TARGET SFML::graphics)
    message(STATUS "SFML (3.0.1) fetched and made available via FetchContent.")
else()
    message(STATUS "SFML found via find_package (Version: ${SFML_VERSION}). Component 'graphics' was requested.")
    if (TARGET SFML::graphics)
        set(SFML_LINK_TARGET SFML::graphics)
    else()
        message(FATAL_ERROR "SFML 3.0.x found (with component 'graphics'), but target SFML::graphics is not available. Check SFML installation or CMake modules.")
    endif()
    message(STATUS "SFML link target set to: ${SFML_LINK_TARGET}")
endif()

# ... (rest of the CMakeLists.txt remains the same) ...

# --- Executable ---
add_executable(${EXECUTABLE_NAME} src/main.cpp)

# Link SFML
if(NOT SFML_LINK_TARGET)
    message(FATAL_ERROR "SFML_LINK_TARGET was not set. SFML setup failed.")
elseif(NOT TARGET ${SFML_LINK_TARGET})
    message(FATAL_ERROR "SFML link target '${SFML_LINK_TARGET}' is not a known target. SFML setup failed.")
else()
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${SFML_LINK_TARGET})
    message(STATUS "Linking against SFML target: ${SFML_LINK_TARGET}")
endif()

# --- Asset Handling ---
if(EXISTS "${CMAKE_SOURCE_DIR}/arial.ttf")
    configure_file(
        "${CMAKE_SOURCE_DIR}/arial.ttf"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/arial.ttf"
        COPYONLY
    )
    message(STATUS "Copied arial.ttf to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/arial.ttf")
else()
    message(WARNING "arial.ttf not found in ${CMAKE_SOURCE_DIR}/. It will not be copied.")
endif()

# --- Platform-specific handling info ---
if(WIN32 AND DEFINED SFML_SOURCE_DIR AND NOT SFML_STATIC_LIBRARIES)
    message(STATUS "On Windows with FetchContent SFML: Ensure SFML DLLs are available for runtime.")
    message(STATUS "SFML (if built by FetchContent) DLLs might be in a subdirectory of '${SFML_BINARY_DIR}'.")
endif()

if(APPLE AND DEFINED SFML_SOURCE_DIR)
    message(STATUS "On macOS with FetchContent SFML: SFML will be built from source.")
elif(APPLE AND SFML_FOUND AND NOT DEFINED SFML_SOURCE_DIR)
    message(STATUS "On macOS: Using SFML (${SFML_VERSION}) found via Homebrew.")
endif()