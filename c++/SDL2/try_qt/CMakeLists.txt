cmake_policy(VERSION 3.9)
project(try_qt VERSION 0.1)
cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core Gui Qml Quick REQUIRED)

file(GLOB SOURCES ./*.cpp ./*.h ./*.c ./*.ui)

add_executable(${PROJECT_NAME} ${SOURCES})



find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
    set(SDL2_LIBRARY /usr/lib/x86_64-linux-gnu/libSDL2.so)
    list(APPEND LINK_LIB_LIST ${SDL2_LIBRARY})
    message(STATUS "found SDL2:${SDL2_INCLUDE_DIR},${SDL2_LIBRARY}")
endif(SDL2_FOUND)
set (SDL2_IMAGE_LIBRARY /usr/lib/x86_64-linux-gnu/libSDL2_image.so)
list(APPEND LINK_LIB_LIST ${SDL2_IMAGE_LIBRARY})



 target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick ${LINK_LIB_LIST})
