cmake_minimum_required(VERSION 3.1)

project(Seeker_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-rpath=./lib/")

set(CMAKE_INSTALL_PREFIX ./build)

# find opencv
find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "opencv Not Found!")
else ()
    message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
    message(STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
endif (NOT OpenCV_FOUND)

find_package(OpenCV REQUIRED)

include_directories(./include)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

file(GLOB_RECURSE SOURCES "./src/*.cpp" "./src/*.c" "./src/**/*.cpp" "./src/**/*.c")

add_executable(Seeker ${SOURCES})

target_link_libraries(Seeker PUBLIC ${OpenCV_LIBS} ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} -pthread)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# 添加链接库
target_link_libraries(Seeker PUBLIC
    ${OpenCV_LIBS}
    ${CMAKE_DL_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${PC_LIBAVCODEC_LIBRARIES}
    ${PC_LIBAVUTIL_LIBRARIES}
    stdc++fs
    pthread
)


install(TARGETS Seeker DESTINATION .)
