# Client executable (rendering / input / audio). raylib lives ONLY here.
# (raylib target is provided by FetchContent in the top-level CMakeLists.txt.)
add_executable(client
    src/main.cpp
    src/Game.cpp
    src/AssetManager.cpp
    src/GameCamera.cpp
    src/Renderer.cpp
    src/Input.cpp
    src/Audio.cpp
)

target_include_directories(client PRIVATE src)
target_link_libraries(client PRIVATE raylib shared)
