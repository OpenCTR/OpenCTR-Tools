######################################################################
# This file is part of OpenCTR.
# 
# OpenCTR is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# OpenCTR is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with OpenCTR.  If not, see <http://www.gnu.org/licenses/>.
######################################################################

# Beta version of new ctrxtool.
# Not ready for production use yet.

set(CTRXTOOL_VERSION_MAJOR 0)
set(CTRXTOOL_VERSION_MINOR 0)
set(CTRXTOOL_VERSION_PATCH 1)
set(CTRXTOOL_VERSION "${CTRXTOOL_VERSION_MAJOR}.${CTRXTOOL_VERSION_MINOR}.${CTRXTOOL_VERSION_PATCH}")

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)

find_package(LibELF)

if(LIBELF_FOUND)
	set(HAVE_LIBELF_H TRUE)
endif()

if(LIBELF_HAVE_GELF)
	set(HAVE_GELF_H TRUE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(DEBUG TRUE)
endif()

check_include_file("stdio.h" HAVE_STDIO_H)
check_include_file("stdlib.h" HAVE_STDLIB_H)
check_include_file("string.h" HAVE_STRING_H)
check_include_file("stdint.h" HAVE_STDINT_H)
check_include_file("unistd.h" HAVE_UNISTD_H)
check_include_file("sys/stat.h" HAVE_STAT_H)
check_include_file("fcntl.h" HAVE_FCNTL_H)
check_include_file("assert.h" HAVE_ASSERT_H)
check_include_file("libgen.h" HAVE_LIBGEN_H)
check_include_file("limits.h" HAVE_LIMITS_H)
check_include_file("errno.h" HAVE_ERRNO_H)

check_include_file("getopt.h" HAVE_GETOPT_H)
if(HAVE_GETOPT_H)
	check_symbol_exists("getopt_long" "getopt.h" HAVE_GETOPT)
else()
	check_symbol_exists("getopt_long" "unistd.h" HAVE_GETOPT)
endif()

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
	${CMAKE_CURRENT_BINARY_DIR}/config.h
	@ONLY
)

add_definitions(-DHAVE_CONFIG_H)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${LIBELF_INCLUDE_DIR}
)

add_executable(ctrxtool
	source/main.c 
	source/ctrx.c 
	source/elf.c 
	source/utils.c 
)

target_link_libraries(ctrxtool
	${LIBELF_LIBRARY}
)

install(TARGETS ctrxtool 
	EXPORT ctrxtool
	RUNTIME DESTINATION bin
)

if(WIN32)
	# Win32 CMake find_package() searches <prefix>/CMake/
	install(EXPORT ctrxtool
		DESTINATION CMake
		FILE ctrxtool.cmake
	)
else()
	# UNIX CMake find_package() searches <prefix>/lib/cmake
	install(EXPORT ctrxtool
		DESTINATION lib/cmake
		FILE ctrxtool.cmake
	)
endif()
