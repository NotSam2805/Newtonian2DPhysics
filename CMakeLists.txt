cmake_minimum_required(VERSION 3.20)

project(N2P
    VERSION 1.0
    LANGUAGES CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

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
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(N2P PUBLIC cxx_std_20)

set(N2P_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/N2P")

install(TARGETS N2P
    EXPORT N2PTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/n2p
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_package_config_file(
    cmake/N2PConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/N2PConfig.cmake"
    INSTALL_DESTINATION "${N2P_INSTALL_CMAKEDIR}"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/N2PConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(EXPORT N2PTargets
    FILE N2PTargets.cmake
    NAMESPACE N2P::
    DESTINATION "${N2P_INSTALL_CMAKEDIR}"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/N2PConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/N2PConfigVersion.cmake"
    DESTINATION "${N2P_INSTALL_CMAKEDIR}"
)

# =========================
# Demo
# =========================

option(N2P_BUILD_DEMO "Build the SDL2 demo executable" ON)

if(N2P_BUILD_DEMO)
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
endif()