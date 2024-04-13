cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME webBrowser)

project(${PROJECT_NAME} LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# link against SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Link against SDL_ttf
set(SDL2_TTF_DIRS "/opt/homebrew/Cellar/sdl2_ttf/2.22.0/include/SDL2")
set(SDL2_TTF_LIBRARIES "/opt/homebrew/Cellar/sdl2_ttf/2.22.0/lib/libSDL2_ttf.dylib")
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIRS})

# Define the executable target
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

# link against SDL2
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})

# link against SDL_ttf
target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIBRARIES})

# Collect all .cpp files in the src directory
file(GLOB_RECURSE PROJECT_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Set sources for the target, excluding the main.cpp file
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})

# Set include directories for the target
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)