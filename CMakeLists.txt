cmake_minimum_required (VERSION 3.16)

project("libdbcppp"  VERSION 1.0 LANGUAGES CXX)

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

#find_package(Boost 1.69.0 REQUIRED COMPONENTS program_options)
include_directories(D:/boost_1_69_0)
link_directories(D:/boost_1_69_0/stage/lib)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	add_definitions("/bigobj")
endif()

add_subdirectory(src)


