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

# Import of 3dsxtool from https://github.com/devkitPro/3dstools
# Thanks to fincs for creating this.

include(CMakePackageConfigHelpers)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(PACKAGE_NAME "3dsxtool")
set(PACKAGE_AUTHOR "fincs")
set(PACKAGE_DESCRIPTION "Tool for converting ELF to 3DSX.")
set(PACKAGE_VERSION_MAJOR 1)
set(PACKAGE_VERSION_MINOR 0)
set(PACKAGE_VERSION_PATCH 4)
set(PACKAGE_VERSION ${PACKAGE_VERSION_MAJOR}.${PACKAGE_VERSION_MINOR}.${PACKAGE_VERSION_PATCH})

add_executable(3dsxtool
	src/3dsxtool.cpp
)

if(WIN32)
	# Win32 CMake find_package() searches <prefix>/CMake/
	set(CONFIG_PREFIX "CMake")
else()
	# Unix CMake find_package() searches <prefix>/share/cmake
	set(CONFIG_PREFIX "share/cmake")
endif()

configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/3dsxtool-config.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/3dsxtool-config.cmake
	INSTALL_DESTINATION ${CONFIG_PREFIX}
	PATH_VARS CONFIG_PREFIX
)

install(TARGETS 3dsxtool 
	EXPORT 3dsxtool 
	RUNTIME DESTINATION bin
)

install(EXPORT 3dsxtool 
	DESTINATION ${CONFIG_PREFIX}
	FILE 3dsxtool-targets.cmake
	NAMESPACE CTR::
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/3dsxtool-config.cmake
	DESTINATION ${CONFIG_PREFIX}
)
