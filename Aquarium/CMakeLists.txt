cmake_minimum_required(VERSION 3.20)
project(Aquarium LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML installed via Homebrew (brew install sfml)
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

# Collect all source files under src/ (add new .cpp files here as the labs grow,
# or switch to file(GLOB_RECURSE ...) later if preferred)
add_executable(Aquarium
    main.cpp
)

target_link_libraries(Aquarium PRIVATE SFML::Graphics SFML::Window SFML::System)

# Helpful compiler warnings while developing
target_compile_options(Aquarium PRIVATE -Wall -Wextra)
