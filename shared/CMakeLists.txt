# Pure simulation library: entities, movement, combat, tick.
# IMPORTANT: no raylib and no networking here -- the server reuses this unchanged.
add_library(shared STATIC
    src/World.cpp
    src/entities/Hero.cpp
    src/entities/Projectile.cpp
)

target_include_directories(shared PUBLIC include)
target_compile_features(shared PUBLIC cxx_std_17)
