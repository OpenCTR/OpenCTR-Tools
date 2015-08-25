#########################################################################
# OpenCTR-Tools - Tools used in the OpenCTR SDK.
# 
# Copyright (C) 2015 The OpenCTR Project. 
# 
# This file is part of OpenCTR-Tools. 
# 
# OpenCTR-Tools is free software: you can redistribute it and/or modify 
# it under the terms of the GNU General Public License version 3 as 
# published by the Free Software Foundation.
# 
# OpenCTR-Tools is distributed in the hope that it will be useful, 
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License 
# along with OpenCTR-Tools. If not, see <http://www.gnu.org/licenses/>. 
#########################################################################

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CMakePackageConfigHelpers)

find_package(LibELF QUIET REQUIRED)

if(LIBELF_FOUND)
	set(HAVE_LIBELF_H TRUE)
endif()

if(LIBELF_HAVE_GELF)
	set(HAVE_GELF_H TRUE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG TRUE)
endif()

check_include_file("assert.h" HAVE_ASSERT_H)
check_include_file("errno.h" HAVE_ERRNO_H)
check_include_file("fcntl.h" HAVE_FCNTL_H)
check_include_file("getopt.h" HAVE_GETOPT_H)
check_include_file("libgen.h" HAVE_LIBGEN_H)
check_include_file("stdint.h" HAVE_STDINT_H)
check_include_file("stdio.h" HAVE_STDIO_H)
check_include_file("stdlib.h" HAVE_STDLIB_H)
check_include_file("string.h" HAVE_STRING_H)
check_include_file("sys/stat.h" HAVE_SYSSTAT_H)
check_include_file("unistd.h" HAVE_UNISTD_H)
check_function_exists("getopt" HAVE_GETOPT_SHORT)
check_function_exists("getopt_long" HAVE_GETOPT_LONG)

set(PACKAGE_NAME "artool")
set(PACKAGE_AUTHOR "Symbitic")
set(PACKAGE_DESCRIPTION "Convert resource files into ARM resources.")
set(PACKAGE_VERSION 1.0.0)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h
	@ONLY
)

add_definitions(-DHAVE_CONFIG_H)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include_directories(
	${LIBELF_INCLUDE_DIR}
)

add_executable(artool
	src/main.c 
)

target_link_libraries(artool
	${LIBELF_LIBRARY}
)

# Configure artool-config.cmake
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/artool-config.cmake.in 
	${CMAKE_CURRENT_BINARY_DIR}/artool-config.cmake 
	INSTALL_DESTINATION lib/cmake/artool
)

# Create artool-config-version.cmake
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/artool-config-version.cmake 
	COMPATIBILITY AnyNewerVersion 
	VERSION ${PACKAGE_VERSION}
)

# Install artool
install(TARGETS artool 
	EXPORT artool 
	RUNTIME DESTINATION bin
)

# Install the artool CMake Export
install(EXPORT artool 
	DESTINATION lib/cmake/artool
	FILE artool-targets.cmake
	NAMESPACE CTR::
)

# Install artool-config.cmake
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/artool-config.cmake 
	DESTINATION lib/cmake/artool
)

# Install artool-config-version.cmake
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/artool-config-version.cmake 
	DESTINATION lib/cmake/artool
)
