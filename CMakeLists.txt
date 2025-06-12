cmake_minimum_required(VERSION 3.13)
project(forestGame)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/dir/*.cpp"
)

set(SOURCE_FILES
    src/dir/BulletDebugDrawer.cpp
    src/dir/Minimap.cpp
    ${SOURCES}
)

message(STATUS "Sources: ${SOURCE_FILES}")

file(GLOB HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.hpp"
)

# Ici juste Bites
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

add_executable(forest ${SOURCE_FILES} ${HEADERS})

target_include_directories(forest PRIVATE 
    ${OGRE_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/dir
)

target_compile_definitions(forest PRIVATE ${OGRE_DEFINITIONS})

target_link_directories(forest PRIVATE ${OGRE_LIBRARY_DIRS})
target_link_libraries(forest PRIVATE ${OGRE_LIBRARIES})
target_link_libraries(forest PRIVATE OgreBites OgreMain)
target_link_libraries(forest PRIVATE  
    BulletDynamics
    BulletCollision
    LinearMath
)

# Copy resources to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/media DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/configs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
