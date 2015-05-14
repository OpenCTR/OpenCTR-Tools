###############################################################################
# OpenCTR-Tools - Tools used by the OpenCTR SDK.
# 
# Copyright (C) 2015 Symbitic (and other contributing authors)
# 
# This file is part of OpenCTR-Tools.
# 
# OpenCTR-Tools is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the 
# Free Software Foundation, either version 3, or (at your option) 
# any later version.
# 
# OpenCTR-Tools is distributed in the hope that it will be useful, but 
# WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along with 
# OpenCTR-Tools; see the accompanying file LICENSE.txt for details. If not, 
# see <http://www.gnu.org/licenses/>.
###############################################################################

# Import of 3dsxtool from https://github.com/devkitPro/3dstools
# Thanks to fincs for creating this.

include(CMakeExportPackage)

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

# Generate 3dsxtool-config.cmake and 3dsxtool-config-version.cmake
cmake_export_package(
	INPUT_FILE 3dsxtool-config.cmake.in
	OUTPUT_FILE CMakeFiles/3dsxtool-config.cmake
	VERSION_FILE CMakeFiles/3dsxtool-config-version.cmake
	INSTALL_DESTINATION lib/cmake/3dsxtool
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

# Install 3dsxtool-config.cmake and 3dsxtool-config-version.cmake
install(FILES 
	${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/3dsxtool-config.cmake
	${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/3dsxtool-config-version.cmake
	DESTINATION lib/cmake/3dsxtool
)

