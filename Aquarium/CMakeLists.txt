cmake_minimum_required(VERSION 3.20)
project(aquarium LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML installed via Homebrew (brew install sfml)
find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

# Collect all source files under src/ (add new .cpp files here as the labs grow,
# or switch to file(GLOB_RECURSE ...) later if preferred)
add_executable(aquarium
    main.cpp
)

target_link_libraries(aquarium PRIVATE sfml-graphics sfml-window sfml-system)

# Helpful compiler warnings while developing
target_compile_options(aquarium PRIVATE -Wall -Wextra)
