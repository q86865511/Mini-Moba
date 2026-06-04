# Top-level build for the mini MOBA project.
cmake_minimum_required(VERSION 3.21)
project(mini_moba LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Dependencies ---
# raylib: pinned to 5.5 via FetchContent.
# Why not vcpkg here: vcpkg's current raylib (6.0) has a broken render loop in
# this environment -- EndDrawing() does not swap/poll/wait, so the window stays
# white and "Not Responding" while the loop spins at ~400k fps. Downgrading via
# vcpkg is blocked by our shallow vcpkg clone, so we fetch the proven 5.5 here.
# (vcpkg is still used for other libs later: enet, lua, sol2, nlohmann-json.)
include(FetchContent)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

# Subprojects. More are added as the phases progress:
#   add_subdirectory(shared)   # phase 1: pure simulation library
#   add_subdirectory(server)   # phase 2: authoritative server (ENet)
add_subdirectory(client)
