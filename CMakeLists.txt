# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project("DEMCON-LCA" LANGUAGES CXX)

# Set C++ standard to 20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add strict compiler warnings and sanitizers
# See https://github.com/cpp-best-practices/cppbestpractices/blob/master/02-Use_the_Tools_Available.md#compilers
message(CMAKE_CXX_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /w14640 /WX /permissive-)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /analyze")
else()
	# Enable warnings for Clang and GCC
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-Werror -Weverything -Wno-c++98-compat-pedantic -Wno-padded)
    elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-Werror -Wall -Wextra -Wshadow -Wunused -Wpedantic -Wconversion -Wsign-conversion -Wlogical-op -Wnull-dereference -Wuseless-cast -Wformat=2 -Weffc++)

        # Strip release binary
	    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
    endif()

	# Enable stack canaries
	set(CMAKE_CXX_FLAGS "-fstack-protector")
	# Enable sanitizers on Debug build
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined")
endif()

# Enable link time opimization
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if (ipo_supported)
	set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

option(DISABLE_OUTPUT "Disable output" OFF)
if (DISABLE_OUTPUT)
  MESSAGE(STATUS "DISABLE_OUTPUT: ON")
  add_definitions(-DDISABLE_OUTPUT)
endif()

IF (NOT WIN32)
    # `ctest`
    enable_testing()
    add_subdirectory(tests)

    # cppcheck and clang-tidy
    option(STATIC_ANALYSIS "Run static code analysis" OFF)
    message(STATUS "STATIC_ANALYSIS: ${STATIC_ANALYSIS}")

    if(STATIC_ANALYSIS)
        find_program(CPPCHECK cppcheck)
        message(STATUS "cppcheck ${CPPCHECK}")
        if(CPPCHECK)
            set(CMAKE_CXX_CPPCHECK "${CPPCHECK}"
                "--enable=all"
                "--inconclusive"
                "--inline-suppr"
                "--quiet"
                "--suppress=unmatchedSuppression"
                "--suppress=unusedFunction")
        endif()

        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            find_program(CLANG_TIDY clang-tidy)
            message(STATUS "clang-tidy ${CLANG_TIDY}")

            if(CLANG_TIDY)
                set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY}"
                        "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
            endif()
        endif()

        if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
            add_compile_options(-fanalyzer)
        endif()
    endif(STATIC_ANALYSIS)
ENDIF()

file(GLOB SOURCES "src/*.cpp")
add_executable(lca ${SOURCES})