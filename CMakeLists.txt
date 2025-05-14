cmake_minimum_required(VERSION 3.28)
project(ProjectT) # Or your actual project name

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)


include(FetchContent)
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG        3.0.0 # 
)
FetchContent_MakeAvailable(sfml)

# Define your executable and list ALL .cpp source files
add_executable(project-t 
    src/main.cpp
    src/CollisionSystem.cpp   
    src/playerbody.cpp        
    src/platformbody.cpp    # Uncomment if the file exists
    src/tile.cpp
    src/trapbody.cpp        # If you have it
    src/Updater.cpp         # If you have it
)

# Tell CMake where to find your project's include files (.hpp)
target_include_directories(project-t PUBLIC 
    ${PROJECT_SOURCE_DIR}/include      
)


target_link_libraries(project-t PRIVATE # Or your executable name
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
    sfml-network
)