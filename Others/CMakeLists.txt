cmake_minimum_required(VERSION 3.16)
project(TypingMasterMVC)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(
    include/model
    include/view
    include/controller
)

# Source files
set(SOURCES
    src/main.cpp
    src/model/GameModel.cpp
    src/view/GameView.cpp
    src/controller/GameController.cpp
)

add_executable(TypingMasterMVC ${SOURCES})

# Link raylib (adjust path if needed)
find_package(raylib REQUIRED)
target_link_libraries(TypingMasterMVC raylib)
