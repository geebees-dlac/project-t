cmake_minimum_required(VERSION 3.10)

# Set project name and C++ standard
project(MyCrossPlatformApp LANGUAGES CXX)   
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Platform-specific configurations
if(WIN32)
    message(STATUS "Configuring for Windows")
    # Windows-specific flags or libraries
    # Example: target_link_libraries(${PROJECT_NAME} ws2_32)
elseif(APPLE)
    message(STATUS "Configuring for macOS")
    # macOS-specific settings
    # Example: link to frameworks if needed
    # target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
endif()
