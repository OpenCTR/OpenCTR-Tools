################################################################################
# OpenCTR-Tools - Tools used in the OpenCTR SDK.
#
# Copyright (C) 2015 The OpenCTR Project.
#
# This file is part of OpenCTR-Tools.
#
# OpenCTR-Tools is free software: you can redistribute it and/or modify it
# under the terms of the GNU General Public License version 3 as published by
# the Free Software Foundation.
#
# OpenCTR-Tools is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
#
# You should have received a copy of the GNU General Public License
# along with OpenCTR-Tools. If not, see <http://www.gnu.org/licenses/>.
################################################################################

include(CheckFunctionExists)
include(CheckIncludeFile)

project(artool)

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

add_executable(artool
    src/main.c
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

target_compile_options(artool PRIVATE
    -DHAVE_CONFIG_H
)

target_include_directories(artool PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LIBELF_INCLUDE_DIR}
)

target_link_libraries(artool PRIVATE
    ${LIBELF_LIBRARY}
)

install(TARGETS artool
    EXPORT artool
    RUNTIME DESTINATION bin
)

install(EXPORT artool
    DESTINATION share/artool/cmake
    FILE artool-targets.cmake
    NAMESPACE CTR::
)

install(FILES artool-config.cmake
    DESTINATION share/artool/cmake)
