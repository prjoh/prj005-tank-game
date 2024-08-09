cmake_minimum_required(VERSION 3.25)

project(PRJ005 LANGUAGES CXX)

# Set the source directory
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/Source")

# Check the platform and set the project directories accordingly
if (CMAKE_GENERATOR_PLATFORM STREQUAL "x64")
    set(BINARY_DIR "${CMAKE_SOURCE_DIR}/Bin/x64")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(BINARY_DIR "${CMAKE_SOURCE_DIR}/Bin/linux")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(BINARY_DIR "${CMAKE_SOURCE_DIR}/Bin/osx")
# TODO: Quit for unknown platform
endif()

# set(BOX2D_BUILD_UNIT_TESTS OFF)
# set(BOX2D_BUILD_TESTBED OFF)
# add_compile_options("-DBOX2D_BUILD_UNIT_TESTS=OFF")
# add_compile_options("-DBOX2D_BUILD_TESTBED=OFF")

# Include the CMakeLists.txt for dependencies
add_subdirectory(${SOURCE_DIR}/ThirdParty/fmt)
add_subdirectory(${SOURCE_DIR}/ThirdParty/pugixml)
add_subdirectory(${SOURCE_DIR}/ThirdParty/box2d)
add_subdirectory(${SOURCE_DIR}/ThirdParty/raylib)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(SRC_ROOT
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/AssetManager.cpp
    ${SOURCE_DIR}/AssetManager.h
    ${SOURCE_DIR}/Assets.cpp
    ${SOURCE_DIR}/Assets.h
    ${SOURCE_DIR}/Component.h
    ${SOURCE_DIR}/Config.h
    ${SOURCE_DIR}/Entity.cpp
    ${SOURCE_DIR}/Entity.h
    ${SOURCE_DIR}/Error.cpp
    ${SOURCE_DIR}/Error.h
    ${SOURCE_DIR}/Game.cpp
    ${SOURCE_DIR}/Game.h
    ${SOURCE_DIR}/GameData.cpp
    ${SOURCE_DIR}/GameData.h
    ${SOURCE_DIR}/Guid.cpp
    ${SOURCE_DIR}/Guid.h
    ${SOURCE_DIR}/MathExtension.cpp
    ${SOURCE_DIR}/MathExtension.h
    ${SOURCE_DIR}/ObjectPool.h
    ${SOURCE_DIR}/Platforms.h
    ${SOURCE_DIR}/Service.cpp
    ${SOURCE_DIR}/Service.h
    ${SOURCE_DIR}/Singleton.h
    ${SOURCE_DIR}/System.cpp
    ${SOURCE_DIR}/System.h
    ${SOURCE_DIR}/Types.h
    ${SOURCE_DIR}/Utils.h
)
set(SRC_SYSTEMS
    ${SOURCE_DIR}/Systems/CameraSystem.cpp
    ${SOURCE_DIR}/Systems/CameraSystem.h
    ${SOURCE_DIR}/Systems/CommandSystem.cpp
    ${SOURCE_DIR}/Systems/CommandSystem.h
    ${SOURCE_DIR}/Systems/FSMSystem.cpp
    ${SOURCE_DIR}/Systems/FSMSystem.h
    ${SOURCE_DIR}/Systems/GraphicsAnimator.cpp
    ${SOURCE_DIR}/Systems/GraphicsAnimator.h
    ${SOURCE_DIR}/Systems/ParticleSystem.cpp
    ${SOURCE_DIR}/Systems/ParticleSystem.h
    ${SOURCE_DIR}/Systems/PhysicsSystem.cpp
    ${SOURCE_DIR}/Systems/PhysicsSystem.h
    ${SOURCE_DIR}/Systems/PlayerController.cpp
    ${SOURCE_DIR}/Systems/PlayerController.h
    ${SOURCE_DIR}/Systems/PointerAim.cpp
    ${SOURCE_DIR}/Systems/PointerAim.h
    ${SOURCE_DIR}/Systems/ProjectileSystem.cpp
    ${SOURCE_DIR}/Systems/ProjectileSystem.h
    ${SOURCE_DIR}/Systems/RenderSystem.cpp
    ${SOURCE_DIR}/Systems/RenderSystem.h
    ${SOURCE_DIR}/Systems/RotateToAim.cpp
    ${SOURCE_DIR}/Systems/RotateToAim.h
)
set(SRC_COMPONENTS
    ${SOURCE_DIR}/Components/ActorCommand.h
    ${SOURCE_DIR}/Components/ActorIsMoving.h
    ${SOURCE_DIR}/Components/ActorIsShooting.h
    ${SOURCE_DIR}/Components/AnimationFSM.h
    ${SOURCE_DIR}/Components/Camera.h
    ${SOURCE_DIR}/Components/CameraFollow.h
    ${SOURCE_DIR}/Components/PhysicsComponent.cpp
    ${SOURCE_DIR}/Components/PhysicsComponent.h
    ${SOURCE_DIR}/Components/Command.h
    ${SOURCE_DIR}/Components/ComponentTypes.cpp
    ${SOURCE_DIR}/Components/ComponentTypes.h
    ${SOURCE_DIR}/Components/FilterBits.h
    ${SOURCE_DIR}/Components/FSMData.cpp
    ${SOURCE_DIR}/Components/FSMData.h
    ${SOURCE_DIR}/Components/Graphics.cpp
    ${SOURCE_DIR}/Components/Graphics.h
    ${SOURCE_DIR}/Components/Input.h
    ${SOURCE_DIR}/Components/ParticleSystemComponent.cpp
    ${SOURCE_DIR}/Components/ParticleSystemComponent.h
    ${SOURCE_DIR}/Components/PhysicsWorld.h
    ${SOURCE_DIR}/Components/PlayerCommand.h
    ${SOURCE_DIR}/Components/PointerAim.h
    ${SOURCE_DIR}/Components/Projectile.h
    ${SOURCE_DIR}/Components/RotateToAim.h
    ${SOURCE_DIR}/Components/TileMap.cpp
    ${SOURCE_DIR}/Components/TileMap.h
    ${SOURCE_DIR}/Components/Transform.cpp
    ${SOURCE_DIR}/Components/Transform.h
)
source_group(""           FILES ${SRC_ROOT})
source_group("Systems"    FILES ${SRC_SYSTEMS})
source_group("Components" FILES ${SRC_COMPONENTS})

# Specify the source files
include_directories(${SOURCE_DIR})
add_executable(${PROJECT_NAME}
    ${SRC_ROOT}
    ${SRC_SYSTEMS}
    ${SRC_COMPONENTS}
)

# Set C++ standard version
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Link your project with SDL2 (assuming SDL2 provides CMake targets)
target_link_libraries(${PROJECT_NAME} PRIVATE fmt pugixml box2d raylib)

# Specify the output directory for the executable
# if (DEFINED BINARY_DIR)  # TODO: This is a hack to avoid error for ninja...
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR})
# endif()

# Set the working directory for debugging (for Visual Studio generator)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

if (WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/Assets/
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/Assets/
        VERBATIM)
endif ()
