# Client executable (rendering + input). raylib lives ONLY here.
# (raylib target is provided by FetchContent in the top-level CMakeLists.txt.)
add_executable(client
    src/main.cpp
)

target_link_libraries(client PRIVATE raylib shared)
