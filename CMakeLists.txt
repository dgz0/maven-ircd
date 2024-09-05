# SPDX-License-Identifier: MIT
#
# Copyright 2024 dgz
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the “Software”), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# The minimum CMake version required is determined by the CMake version present
# in the repositories of the latest Ubuntu LTS release still under standard
# support. This is done for several reasons:
#
# - IDEs that ship their own bundled version of CMake should be allowed to
#   configure this project on opening. Since we do not know what version these
#   IDEs bundle, we have to assume the worst case.
#
# - Some Linux distributions actually ship older than expected versions of CMake
#   in their repositories. This policy is proof that at least one major
#   distribution is still supported with a fairly outdated version of CMake.
#
# - Ubuntu was chosen because of its popularity and influence; it is a
#   reasonable benchmark for deciding the minimum versions of applications to
#   support.
#
# As a result, the minimum CMake version required is 3.16.3, which is available
# in the Ubuntu LTS 20.04 "Focal Fossa" repositories. While an exact
# end-of-support date for this version is not known, it is expected to be some
# time in April 2025.
#
# When this happens, the minimum CMake version required should be bumped to the
# CMake version present in the Ubuntu LTS 22.04 "Jammy Jellyfish" repositories.

# The FATAL_ERROR option is accepted but ignored by CMake 2.6 and higher. It
# should be specified so CMake versions 2.4 and lower fail with an error instead
# of just a warning.
cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

project(maven-ircd
	VERSION 1.0.0.0
	DESCRIPTION "Internet Relay Chat daemon (IRCd)"
	HOMEPAGE_URL "https://github.com/dgz0/maven-ircd"
	LANGUAGES C CXX)

if (NOT UNIX)
	message(FATAL_ERROR
		"This project will only compile under UNIX based systems.")
endif()

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(FATAL_ERROR
		"In-source builds are not allowed. Please create a different "
		"build directory and run CMake from there again.")
endif()

# Generates a "compile_commands.json" file containing the exact compiler calls
# for all translation units of the project in machine-readable form. This is
# required for some IDEs and text editors to provide real-time diagnostics and
# and code completion, among other things.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Controls whether to use the FOLDER target property to organize targets into
# folders.
#
# Not all CMake generators support recording folder details for targets. The
# Xcode and Visual Studio generators are examples of generators that do.
# Similarly, not all IDEs support presenting targets using folder hierarchies,
# even if the CMake generator used provides the necessary information.
if (CMAKE_VERSION LESS 3.26)
	# CMake versions >=3.26 treat this property as ON by default.
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# The minimum gcc and clang versions required are bound in the same way as the
# minimum CMake version; see the top of this file for the criterion.
set(CLANG_VER_MIN "10.0.0")
set(GCC_VER_MIN "9.4.0")

option(MAVEN_IRCD_ENABLE_SANITIZERS "Build with ASAN and UBSan" OFF)
option(MAVEN_IRCD_BUILD_UNIT_TESTS "Build the unit tests" OFF)

# Create an interface library that stores the compiler flags we want to pass to
# the compiler call for each target.
#
# An interface library specifies usage requirements for dependencies, but does
# not compile sources or produce a library artefact on disk. Since no sources
# are specified, it is not included as a target in the generated build system.
# However, it can have properties set to it, and can be installed and exported.
add_library(maven-ircd-build-settings-c INTERFACE)
add_library(maven-ircd-build-settings-cpp INTERFACE)

# These compiler flags are supported by both clang and gcc, and are used
# regardless of the desired build type. Of course, we can't formally verify that
# the behaviour of each flag is comparable between the two compilers, but we
# assume it is.
#
# Testing is required when changing this list, or updating the minimum compiler
# versions, to ensure that the above applies.

set(COMPILER_FLAGS_BASE
	-Waggregate-return
	-Wall
	-Walloca
	-Wbad-function-cast
	-Wcast-align
	-Wcast-qual
	-Wconversion
	-Wdate-time
	-Wdisabled-optimization
	-Wdouble-promotion
	-Wextra
	-Wfloat-equal
	-Wformat=2
	-Winit-self
	-Winline
	-Winvalid-pch
	-Wlong-long
	-Wmissing-include-dirs
	-Wmissing-prototypes
	-Wnested-externs
	-Wnull-dereference
	-Wold-style-definition
	-Wpacked
	-Wpadded
	-Wpointer-arith
	-Wredundant-decls
	-Wshadow
	-Wstrict-prototypes
	-Wswitch-default
	-Wswitch-enum
	-Wundef
	-Wuninitialized
	-Wunused-macros
	-Wvla
	-Wwrite-strings)

if (MAVEN_IRCD_ENABLE_SANITIZERS)
	list(APPEND COMPILER_FLAGS_BASE -fsanitize=address,undefined)
	target_link_options(
		maven-ircd-build-settings-c INTERFACE
		-fsanitize=address,undefined
	)

	target_link_options(
		maven-ircd-build-settings-cpp INTERFACE
		-fsanitize=address,undefined
	)
endif()

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
	if (CMAKE_C_COMPILER_VERSION LESS ${GCC_VER_MIN})
		message(FATAL_ERROR
			"Your gcc version is too old; you must use either "
			"gcc >=${GCC_VER_MIN} or clang >=${CLANG_VER_MIN}.")
	endif()

	set(COMPILER_FLAGS_GCC
		-Walloc-zero
		-Wduplicated-branches
		-Wduplicated-cond
		-Wformat-overflow=2
		-Wformat-signedness
		-Wformat-truncation=2
		-Wimplicit-fallthrough=5
		-Wjump-misses-init
		-Wlogical-op
		-Wmissing-profile
		-Wno-aggressive-loop-optimizations
		-Wopenmp-simd
		-Wshift-overflow=2
		-Wstringop-truncation
		-Wsuggest-attribute=cold
		-Wsuggest-attribute=const
		-Wsuggest-attribute=format
		-Wsuggest-attribute=malloc
		-Wsuggest-attribute=noreturn
		-Wsuggest-attribute=pure
		-Wsync-nand
		-Wunsafe-loop-optimizations
		-Wunsuffixed-float-constants
		-Wunused-const-variable=2
		-Wvector-operation-performance)

	target_compile_options(
		maven-ircd-build-settings-c INTERFACE
		${COMPILER_FLAGS_BASE} ${COMPILER_FLAGS_GCC} "-ggdb3"
	)
elseif (CMAKE_C_COMPILER_ID MATCHES "Clang")
	if (CMAKE_C_COMPILER_VERSION LESS ${CLANG_VER_MIN})
		message(FATAL_ERROR
			"Your clang version is too old; you must use either "
			"clang >=${CLANG_VER_MIN} or gcc >=${GCC_VER_MIN}.")
	endif()

	target_compile_options(
		maven-ircd-build-settings-c INTERFACE
		${COMPILER_FLAGS_BASE} "-glldb"
	)
else()
	message(FATAL_ERROR
		"Your C compiler is incompatible with this project; you must "
		"use either clang >=${CLANG_VER_MIN} or gcc >=${GCC_VER_MIN}.")
endif()

add_subdirectory(core)
add_subdirectory(app)
