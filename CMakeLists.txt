cmake_minimum_required(VERSION 3.28) # You're using 3.28, FetchContent benefits from newer CMake
project(ProjectT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF) # Good practice

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_NAME main) # Define your executable name

# --- SFML Handling ---
# Try to find SFML using find_package first (good for macOS/Linux with system installs)
find_package(SFML 3.0 COMPONENTS graphics QUIET) # Using 3.0 as per your FetchContent GIT_TAG
                                                 # QUIET prevents error messages if not found initially

if(NOT SFML_FOUND)
    message(STATUS "SFML not found via find_package. Attempting FetchContent.")
    include(FetchContent)
    FetchContent_Declare(
        SFML # Note: The target name created by FetchContent might be just "SFML" or "sfml"
             # or it might create targets like SFML::Graphics directly.
             # The official SFML CMake export provides SFML::graphics (lowercase g).
             # Let's stick to consistency.
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG        3.0.1 # Using your specified tag for SFML 3.x
        GIT_SHALLOW    ON
        # EXCLUDE_FROM_ALL # Usually good, means it's only built if a target links to it.
        # SYSTEM           # Marks includes from SFML as system includes, suppressing warnings.
                           # With modern SFML, this might not be necessary if it sets INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
    )
    # For SFML 3.x built from source, you might need to set some options
    # if you don't want all default components or want to control dependencies.
    # Example:
    # set(SFML_BUILD_AUDIO OFF CACHE BOOL "Disable SFML Audio module" FORCE)
    # set(SFML_BUILD_NETWORK OFF CACHE BOOL "Disable SFML Network module" FORCE)
    # set(SFML_USE_SYSTEM_DEPS OFF CACHE BOOL "Prefer SFML's bundled dependencies" FORCE) # Can be useful on Windows

    FetchContent_MakeAvailable(SFML)
    # After FetchContent_MakeAvailable(SFML), SFML's targets should be available.
    # For SFML 3.x fetched this way, the target is likely SFML::graphics (lowercase g)
    # or you might link directly to the library targets like sfml-graphics.
    # Let's assume it provides SFML::graphics
    set(SFML_LIBRARIES SFML::graphics) # Define a variable to hold the link target
    message(STATUS "SFML fetched and made available via FetchContent.")
else()
    message(STATUS "SFML found via find_package (Version: ${SFML_VERSION}).")
    # If found by find_package, SFML_LIBRARIES is usually set, or we link to SFML::graphics
    set(SFML_LIBRARIES SFML::graphics) # SFML 2.6+ and 3.x usually provide SFML::graphics
endif()

# --- Executable ---
add_executable(${EXECUTABLE_NAME} src/main.cpp) # Make sure src/main.cpp exists

# target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_17) # Already handled by CMAKE_CXX_STANDARD

# Link SFML
if(TARGET ${SFML_LIBRARIES})
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${SFML_LIBRARIES})
    message(STATUS "Linking against SFML target: ${SFML_LIBRARIES}")
else()
    message(FATAL_ERROR "SFML link target ${SFML_LIBRARIES} not found. SFML setup failed.")
endif()


# --- Asset Handling ---
# Copy arial.ttf to the same directory as the executable
if(EXISTS "${CMAKE_SOURCE_DIR}/arial.ttf")
    configure_file(
        "${CMAKE_SOURCE_DIR}/arial.ttf"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/arial.ttf" # Copy next to executable
        COPYONLY
    )
    message(STATUS "Copied arial.ttf to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/arial.ttf")
else()
    message(WARNING "arial.ttf not found in ${CMAKE_SOURCE_DIR}/. It will not be copied.")
endif()

# --- Platform-specific handling for DLLs (Windows when using FetchContent/dynamic SFML) ---
if(WIN32 AND DEFINED SFML_SOURCE_DIR AND NOT SFML_STATIC_LIBRARIES)
    # When SFML is built by FetchContent, its build artifacts are in SFML_BINARY_DIR
    # SFML_BINARY_DIR is automatically set by FetchContent_MakeAvailable(SFML)
    # The DLLs will be in <SFML_BINARY_DIR>/lib (for Release) or <SFML_BINARY_DIR>/lib/Debug (for Debug)
    # or directly in <SFML_BINARY_DIR>/bin if SFML_INSTALL_BIN_DIR is set that way by SFML's CMake.
    # This is tricky because SFML's own CMake determines where DLLs go during its build.

    # A common location for SFML built from source (like with FetchContent) might be:
    # <SFML_BINARY_DIR>/lib/sfml-graphics-d-3.dll (Debug) or <SFML_BINARY_DIR>/lib/sfml-graphics-3.dll (Release)
    # Let's try to copy all sfml-*.dll and openal32.dll from common output locations.

    # This needs to be a post-build command because the DLLs are only created *after* SFML itself is built.
    # However, for simplicity in this script, we'll try a configure-time copy from where SFML *would*
    # put its DLLs if it were already built. This is not ideal but simpler than a post-build custom command
    # for this example. A better way is `install(RUNTIME_DEPENDENCY_SET ...)` if using CMake 3.21+
    # or custom post-build commands.

    # For now, let's assume SFML puts DLLs in its <SFML_BINARY_DIR>/lib or similar.
    # The user might need to manually copy them if this doesn't work perfectly,
    # or if SFML's build places them elsewhere.
    # With SFML 3.0.1 from FetchContent, the DLLs are often placed directly into the
    # main project's CMAKE_RUNTIME_OUTPUT_DIRECTORY if SFML is built as part of the same configure/build pass.

    # If SFML is built as an external project (which FetchContent does), its DLLs
    # will be in its own build tree: ${SFML_BINARY_DIR}/lib/ (or bin/)
    # We want to copy them to OUR executable's output directory.

    # This is a simplified attempt. Robust DLL handling with FetchContent can be complex.
    # Often, if SFML is built as a shared library by FetchContent, its build rules
    # might already place the DLLs next to your executable if it's part of the same CMake generation pass.
    # If not, you might need to add SFML's DLL output directory to your PATH for running,
    # or manually copy them.

    # Let's try to inform the user if using FetchContent on Windows.
    message(STATUS "On Windows with FetchContent SFML: Ensure SFML DLLs (e.g., sfml-graphics-3.dll, openal32.dll) are next to your executable in '${CMAKE_RUNTIME_OUTPUT_DIRECTORY}' or in your system PATH for the application to run.")
    message(STATUS "SFML (if built by FetchContent) DLLs might be found in a subdirectory of '${SFML_BINARY_DIR}' (e.g., lib/ or bin/).")

    # A more proactive (but still potentially fragile) copy attempt for common SFML DLL names:
    # This would ideally be a post-build step.
    # file(GLOB SFML_DLLS
    #     "${SFML_BINARY_DIR}/lib/sfml-*-3.dll"
    #     "${SFML_BINARY_DIR}/lib/Debug/sfml-*-d-3.dll" # If multi-config generator
    #     "${SFML_BINARY_DIR}/lib/openal32.dll"
    # )
    # if(SFML_DLLS)
    #     message(STATUS "Attempting to copy SFML DLLs: ${SFML_DLLS}")
    #     file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    # endif()
endif()

# For macOS, if using FetchContent and building SFML from source,
# it might build .frameworks or .dylibs.
# If find_package is used, Homebrew handles dylib locations.
if(APPLE AND DEFINED SFML_SOURCE_DIR)
    message(STATUS "On macOS with FetchContent SFML: SFML will be built from source. Ensure linkage is correct.")
endif()