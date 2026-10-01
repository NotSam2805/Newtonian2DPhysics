cmake_minimum_required(VERSION 3.20)

project(N2P
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(N2P STATIC

    src/Maths/Vector2.cpp

    src/Physics/Rigidbody.cpp
    src/Physics/PhysicsWorld.cpp
    src/Physics/CollisionDetector.cpp
    src/Physics/CollisionSolver.cpp

    src/Physics/Shapes/Circle.cpp
    src/Physics/Shapes/Polygon.cpp
)

target_include_directories(N2P
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)