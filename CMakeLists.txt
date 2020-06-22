cmake_minimum_required (VERSION 3.16)

project("libdbcppp"  VERSION 1.0 LANGUAGES CXX)

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/Program Files/boost_1_73_0/boost")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/Program Files/boost_1_73_0/lib64-msvc-14.1")

set(CMAKE_STATIC_LIBRARY_PREFIX "")
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

#boost
if (!${Boost_USE_STATIC_LIBS})
	add_definitions(-DBOOST_LOG_DYN_LINK)
endif()

find_package(Boost 1.73 REQUIRED COMPONENTS program_options)

add_subdirectory(src)


