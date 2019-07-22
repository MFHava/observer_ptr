
#          Copyright Bernhard Manfred Gruber & Michael Florian Hava.
# Distributed under the Boost Software License, Version 1.0.
#    (See accompanying file LICENSE_1_0.txt or copy at
#          http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.13)
project(ObserverPtr)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/static)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.59 REQUIRED unit_test_framework)
add_library(flags INTERFACE)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(flags INTERFACE -Wall -Wextra -Wpedantic -Wconversion)
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_definitions(flags INTERFACE _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS _SILENCE_FPOS_SEEKPOS_DEPRECATION_WARNING)
	target_compile_options(flags INTERFACE
		/W4 /JMC /MP /permissive- /bigobj
		/wd4003 # ignore not enough parameters for function macro
		/wd4456 # ignore local shadowing
		/wd4457 # ignore function parameter shadowing
		/wd4458 # ignore class member shadowing
		/wd4459 # ignore global shadowing
	)
	target_link_options(flags INTERFACE
		/ignore:4099 # ignore mixing struct and class keyword for same type
	)
endif()

file(GLOB_RECURSE HEADERS "inc/*")
file(GLOB_RECURSE TESTS "test/*")
add_executable(tests ${HEADERS} ${TESTS}) #including headers here to have them included in IDEs
target_include_directories(tests PRIVATE "inc")
target_link_libraries(tests flags Boost::unit_test_framework)
