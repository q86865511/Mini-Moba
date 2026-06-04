# Client executable (rendering / input / audio). raylib lives ONLY here.
# (raylib target is provided by FetchContent in the top-level CMakeLists.txt.)
add_executable(client
    src/main.cpp
    src/Game.cpp
    src/AssetManager.cpp
    src/AnimatedSprite.cpp
    src/EntityView.cpp
    src/Effects.cpp
    src/GameCamera.cpp
    src/Renderer.cpp
    src/Input.cpp
    src/Audio.cpp
)

find_package(nlohmann_json CONFIG REQUIRED)

target_include_directories(client PRIVATE src)
target_link_libraries(client PRIVATE raylib shared nlohmann_json::nlohmann_json)

# Copy the assets/ folder next to the built exe so it can load them at runtime.
add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:client>/assets
    COMMENT "Copying assets to output directory")
