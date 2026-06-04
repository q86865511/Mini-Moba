# Client executable (rendering + input). Raylib lives ONLY here.
find_package(raylib CONFIG REQUIRED)

add_executable(client
    src/main.cpp
)

target_link_libraries(client PRIVATE raylib)

# Phase 1+ will link the shared simulation library:
#   target_link_libraries(client PRIVATE shared)
