cmake_minimum_required(VERSION 3.10)
project(ForestZ)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Ogre REQUIRED)
find_package(Bullet REQUIRED)

include_directories(${OGRE_INCLUDE_DIRS} ${BULLET_INCLUDE_DIRS})

add_executable(ForestZ
    src/dir/main.cpp
    src/dir/forest.cpp
    src/dir/WelcomePage.cpp
)

target_link_libraries(ForestZ
    ${OGRE_LIBRARIES}
    ${BULLET_LIBRARIES}
)

# Copier les ressources
file(COPY ${CMAKE_SOURCE_DIR}/media DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/configs DESTINATION ${CMAKE_BINARY_DIR})
