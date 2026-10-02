cmake_minimum_required(VERSION 3.20)

project(N2P
    VERSION 1.0
    LANGUAGES CXX
)

# =========================
# N2P Library
# =========================

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(N2P STATIC

    src/Maths/Vector2.cpp
    src/Maths/MathsUtils.cpp

    src/Physics/Rigidbody.cpp
    src/Physics/PhysicsWorld.cpp

    src/Shapes/Circle.cpp
    src/Shapes/Polygon.cpp
    src/Shapes/Rect.cpp

    src/Collision/CollisionDetector.cpp
    src/Collision/CollisionSolver.cpp
)

target_include_directories(N2P
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

# =========================
# Demo
# =========================

find_package(SDL2 CONFIG REQUIRED)

add_executable(N2Pdemo
    Examples/demo.cpp
    Examples/Rendering/Renderer.cpp
)

target_link_libraries(N2Pdemo PRIVATE 
    N2P
    SDL2::SDL2main
    SDL2::SDL2
)