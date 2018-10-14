cmake_minimum_required(VERSION 3.5)
project(lime)

set(CMAKE_CXX_STANDARD 11)

find_package(LimeSuite REQUIRED)
message(STATUS ${LimeSuite_LIBRARIES})
message(STATUS ${LimeSuite_INCLUDE_DIRS})
include_directories(${LimeSuite_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

# Boost
set(Boost_USE_STATIC_LIBS ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS OFF) # ignore debug libs and
set(Boost_USE_RELEASE_LIBS ON)  # only find release libs
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS system thread REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif ()

add_subdirectory(include)
add_subdirectory(lib)


add_executable(LimeRun main.cpp)
target_link_libraries(LimeRun ${Boost_LIBRARIES} LimeSDR)


add_executable(basicRx basicRx.cpp)
target_link_libraries(basicRx ${Boost_LIBRARIES} LimeSDR)


enable_testing()
add_subdirectory(tests)