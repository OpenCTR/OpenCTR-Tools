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
set(PACKAGE_VERSION 1.0.4)

add_executable(3dsxtool
	src/3dsxtool.cpp
)

# Configure 3dsxtool-config.cmake
configure_package_config_file(
	${CMAKE_CURRENT_SOURCE_DIR}/3dsxtool-config.cmake.in 
	${CMAKE_CURRENT_BINARY_DIR}/3dsxtool-config.cmake 
	INSTALL_DESTINATION lib/cmake/3dsxtool
)

# Create 3dsxtool-config-version.cmake
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/3dsxtool-config-version.cmake 
	COMPATIBILITY AnyNewerVersion 
	VERSION ${PACKAGE_VERSION}
)

# Install 3dsxtool
install(TARGETS 3dsxtool 
	EXPORT 3dsxtool 
	RUNTIME DESTINATION bin
)

# Install the 3dsxtool CMake Export
install(EXPORT 3dsxtool 
	DESTINATION lib/cmake/3dsxtool
	FILE 3dsxtool-targets.cmake
	NAMESPACE CTR::
)

# Install 3dsxtool-config.cmake
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/3dsxtool-config.cmake 
	DESTINATION lib/cmake/3dsxtool
)

# Install 3dsxtool-config-version.cmake
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/3dsxtool-config-version.cmake 
	DESTINATION lib/cmake/3dsxtool
)
