cmake_minimum_required(VERSION 3.10)

project(project-t)

# Find SFML package
find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Network)

# Add include directories
include_directories(${SFML_INCLUDE_DIRS})

# Link SFML libraries
add_executable(project-t src/main.cpp)
target_link_libraries(project-t PRIVATE SFML::Graphics SFML::Audio SFML::Network)



