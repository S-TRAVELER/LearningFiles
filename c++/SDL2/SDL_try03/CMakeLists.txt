project(SDL_try03)

#查找SDL2是否安装
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

find_package(SDL2_image QUIET)
if(SDL2_IMAGE_FOUND)
    include_directories(${SDL2_IMAGE_INCLUDE_DIR})
    list(APPEND LINK_LIB_LIST ${SDL2_IMAGE_LIBRARY})
    message(STATUS "found SDL2_image:${SDL2_IMAGE_INCLUDE_DIR},${SDL2_IMAGE_LIBRARY}")
else(SDL2_IMAGE_FOUND)
    set (SDL2_IMAGE_LIBRARY /usr/lib/x86_64-linux-gnu/libSDL2_image.so)
    list(APPEND LINK_LIB_LIST ${SDL2_IMAGE_LIBRARY})
    message(STATUS "found SDL2_image:${SDL2_IMAGE_INCLUDE_DIR},${SDL2_IMAGE_LIBRARY}")
endif(SDL2_IMAGE_FOUND)
message(STATUS "linked libraries:${LINK_LIB_LIST}")

find_package(SDL2_TTF QUIET)
if(SDL2_TTF_FOUND)
    include_directories(${SDL2_TTF_INCLUDE_DIR})
    list(APPEND LINK_LIB_LIST ${SDL2_TTF_LIBRARY})
    message(STATUS "found SDL2_image:${SDL2_TTF_INCLUDE_DIR},${SDL2_TTF_LIBRARY}")
else(SDL2_TTF_FOUND)
    set (SDL2_TTF_LIBRARY /usr/lib/x86_64-linux-gnu/libSDL2_ttf.so)
    list(APPEND LINK_LIB_LIST ${SDL2_TTF_LIBRARY})
    message(STATUS "found SDL2_image:${SDL2_TTF_INCLUDE_DIR},${SDL2_TTF_LIBRARY}")
endif(SDL2_TTF_FOUND)

message(STATUS "linked libraries:${LINK_LIB_LIST}")


#set (SDL2_LIBRARY /usr/lib/x86_64-linux-gnu/libSDL2.so)
#set (SDL2_IMAGER_LREBRARY /usr/lib/x86_64-linux-gnu/libSDL2_image.so)
#target_link_libraries(SDL_try03 ${SDL2_LIBRARY} ${SDL2_IMAGER_LREBRARY})
#message (STATUS "linked libraries: "${SDL2_LIBRARY}+${SDL2_IMAGER_LREBRARY})
#install(TARGETS SDL_try03 RUNTIME DESTINATION bin)
add_subdirectory(test)
#add_subdirectory(test_qt)
