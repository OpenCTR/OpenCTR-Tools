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

set(PACKAGE_NAME "artool")
set(PACKAGE_VERSION_MAJOR 0)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_PATCH 1)
set(PACKAGE_VERSION "${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH}")
set(PACKAGE_AUTHOR "Alex Shaw")
set(PACKAGE_DESCRIPTION "Convert resource files into ARM resources.")

include(CheckIncludeFile)

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
check_include_file("libgen.h" HAVE_LIBGEN_H)
check_include_file("stdint.h" HAVE_STDINT_H)
check_include_file("stdio.h" HAVE_STDIO_H)
check_include_file("stdlib.h" HAVE_STDLIB_H)
check_include_file("string.h" HAVE_STRING_H)
check_include_file("sys/stat.h" HAVE_SYSSTAT_H)
check_include_file("unistd.h" HAVE_UNISTD_H)

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

install(TARGETS artool 
	EXPORT artool
	RUNTIME DESTINATION bin
)

if(WIN32)
	# Win32 CMake find_package() searches <prefix>/CMake/
	install(EXPORT artool
		DESTINATION CMake
		FILE artool.cmake
	)
else()
	# UNIX CMake find_package() searches <prefix>/lib/cmake
	install(EXPORT artool
		DESTINATION lib/cmake
		FILE artool.cmake
	)
endif()