set(TEST_SRC mainwindow.cpp)

STRING(REGEX REPLACE "^\\./|\\.c[a-zA-Z0-9_]*$" "" TEST_EXE_NAME ${TEST_SRC})

file(GLOB Header_File ./*.cpp ./*.h ./*.c ./*.ui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR594})
find_package(Qt5 COMPONENTS Core Gui Qml Quick REQUIRED)
message(STATUS "add test:${TEST_EXE_NAME}")
add_library(${Header_File})
add_executable(${TEST_EXE_NAME} ${TEST_SRC})
target_link_libraries(${TEST_EXE_NAME}  Qt5::Core Qt5::Gui Qt5::Qml Qt5::Quick ${LINK_LIB_LIST})
