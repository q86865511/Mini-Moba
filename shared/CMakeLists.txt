# Pure simulation library: entities, movement, combat, match flow.
# IMPORTANT: no raylib and no networking here -- the server reuses this unchanged.
add_library(shared STATIC
    src/World.cpp
    src/Match.cpp
    src/entities/Hero.cpp
    src/entities/Projectile.cpp
    src/entities/Minion.cpp
    src/entities/Tower.cpp
    src/entities/NeutralMonster.cpp
)

target_include_directories(shared PUBLIC include)
target_compile_features(shared PUBLIC cxx_std_17)
