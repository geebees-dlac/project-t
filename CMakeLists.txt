cmake_minimum_required(VERSION 3.10)

# Set project name and C++ standard
project(MySFMLApp LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Platform-specific configuration
if(WIN32)
    message(STATUS "Configuring for Windows")

    # Find SFML on Windows
    find_package(SFML 3.0 REQUIRED graphics window system)


    if(SFML_FOUND)
        message(STATUS "SFML found!")
        target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
    else()
        message(FATAL_ERROR "SFML not found!")
    endif()

elseif(APPLE)
    message(STATUS "Configuring for macOS")

    # On macOS, SFML might need to be found via the system's package manager (e.g., Homebrew)
    find_package(SFML 3.0 REQUIRED graphics window system)

    if(SFML_FOUND)
        message(STATUS "SFML found!")
        target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system "-framework Cocoa" "-framework OpenGL" "-framework IOKit")
    else()
        message(FATAL_ERROR "SFML not found!")
    endif()
endif()

# Other platform-specific adjustments can go here, like defining macros for platform-specific code
