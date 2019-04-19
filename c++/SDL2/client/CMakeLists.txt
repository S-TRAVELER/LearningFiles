cmake_policy(VERSION 3.9)
project(player VERSION 0.1)
cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core Gui Qml Quick REQUIRED)
set (SDL2_LIBRARY /usr/lib/x86_64-linux-gnu/libSDL2.so)
find_package(SDL2 QUIET)
if (SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIR})
    list(APPEND LINK_LIB_LIST ${SDL2_LIBRARY})
    message(STATUS "found SDL2:${SDL2_INCLUDE_DIR},${SDL2_LIBRARY}")
else(SDL2_FOUND)
    list(APPEND LINK_LIB_LIST ${SDL2_LIBRARY})
    message(STATUS "found SDL2:${SDL2_INCLUDE_DIR},${SDL2_LIBRARY}")
endif (SDL2_FOUND)

set(SWSCALE_LIBRARIES /usr/lib/x86_64-linux-gnu/libswscale.so)
include_directories(${SWSCALE_INCLUDE_DIR})
list(APPEND  LINK_LIB_LIST ${SWSCALE_LIBRARIES})
message(STATUS "found  AVCODEC ${SWSCALE_LIBRARIES}")

set(AVCODEC_LIBRARIES /usr/lib/x86_64-linux-gnu/libavcodec.so)
include_directories(${AVCODEC_INCLUDE_DIR})
list(APPEND  LINK_LIB_LIST ${AVCODEC_LIBRARIES})
message(STATUS "found  AVCODEC ${AVCODEC_LIBRARIES}")


set(AVUTIL_LIBRARIES /usr/lib/x86_64-linux-gnu/libavutil.so)
list(APPEND  LINK_LIB_LIST ${AVUTIL_LIBRARIES})
message(STATUS "found  AVUTIL ${AVUTIL_LIBRARIES}")


set(AVFORMAT_LIBRARIES /usr/lib/x86_64-linux-gnu/libavformat.so)
list(APPEND  LINK_LIB_LIST ${AVFORMAT_LIBRARIES})
message(STATUS "found  AVFORMAT ${AVFORMAT_LIBRARIES}")

file(GLOB SOURCES ./*.cpp ./*.h ./*.c ./*.ui)
message(STATUS "found lib: ${LINK_LIB_LIST}")
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick ${LINK_LIB_LIST})
