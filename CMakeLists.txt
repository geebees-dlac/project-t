cmake_minimum_required(VERSION 3.28)
project(ProjectT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_NAME main)

# --- SFML Handling ---
# Option to force FetchContent (e.g., for debugging or if find_package is problematic)
option(PROJECTT_FORCE_SFML_FETCHCONTENT "Force using FetchContent for SFML" OFF)

if(APPLE AND PROJECTT_FORCE_SFML_FETCHCONTENT)
    message(STATUS "PROJECTT_FORCE_SFML_FETCHCONTENT is ON. Forcing FetchContent for SFML on macOS.")
    set(SFML_FOUND FALSE) # Ensure we skip find_package logic if forced
endif()

if(NOT APPLE OR PROJECTT_FORCE_SFML_FETCHCONTENT) # On non-Apple, or if forced on Apple
    # Try find_package first, primarily for Windows if SFML is installed there
    # or if not forcing FetchContent on Apple and want to try find_package.
    # This block will likely be skipped if PROJECTT_FORCE_SFML_FETCHCONTENT is ON for macOS
    if (NOT (APPLE AND PROJECTT_FORCE_SFML_FETCHCONTENT))
        find_package(SFML 3.0 COMPONENTS sfml-graphics QUIET) # Or whatever component name makes sense
    endif()
endif()


if(NOT SFML_FOUND OR (APPLE AND PROJECTT_FORCE_SFML_FETCHCONTENT) )
    if(APPLE AND PROJECTT_FORCE_SFML_FETCHCONTENT)
        message(STATUS "Forcing FetchContent for SFML 3.0.1 on macOS due to PROJECTT_FORCE_SFML_FETCHCONTENT.")
    elseif(NOT SFML_FOUND)
        message(STATUS "SFML not found via find_package. Attempting FetchContent for SFML 3.0.1.")
    endif()

    include(FetchContent)
    FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.1
        GIT_SHALLOW    ON
    )
    # Optional: Control SFML build options if needed for FetchContent
    # set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    # set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    # set(SFML_USE_SYSTEM_DEPS OFF CACHE BOOL "Prefer SFML's bundled dependencies" FORCE) # Often good for Windows

    FetchContent_MakeAvailable(SFML)
    set(SFML_LINK_TARGET SFML::graphics) # SFML 3.x (fetched) provides SFML::graphics
    message(STATUS "SFML (3.0.1) fetched and made available via FetchContent.")
else()
    # This block should only be reached if find_package succeeded AND we are NOT forcing FetchContent on macOS
    message(STATUS "SFML found via find_package (Version: ${SFML_VERSION}).")
    if (TARGET SFML::graphics)
        set(SFML_LINK_TARGET SFML::graphics)
    else()
        message(FATAL_ERROR "SFML found via find_package, but target SFML::graphics is not available. Check SFML installation or CMake modules.")
    endif()
    message(STATUS "SFML link target set to: ${SFML_LINK_TARGET}")
endif()

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
# ... (asset handling remains the same) ...
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
# ... (platform messages remain the same, but note SFML_SOURCE_DIR will now be defined on macOS too if forced) ...
if(WIN32 AND DEFINED SFML_SOURCE_DIR AND NOT SFML_STATIC_LIBRARIES)
    message(STATUS "On Windows with FetchContent SFML: Ensure SFML DLLs are available for runtime.")
    message(STATUS "SFML (if built by FetchContent) DLLs might be in a subdirectory of '${SFML_BINARY_DIR}'.")
endif()

if(APPLE AND DEFINED SFML_SOURCE_DIR)
    message(STATUS "On macOS using FetchContent for SFML: SFML will be built from source.")
elif(APPLE AND SFML_FOUND AND NOT DEFINED SFML_SOURCE_DIR)
    message(STATUS "On macOS: Using SFML (${SFML_VERSION}) found via Homebrew.")
endif()