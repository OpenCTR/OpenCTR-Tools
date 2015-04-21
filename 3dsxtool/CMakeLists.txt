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

# Import of 3dsxtool from devkitPro/3dstools
# Thanks to devkitPro for creating this.

include(CMakePackageConfigHelpers)

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

add_executable(3dsxtool
	3dsxtool.cpp
)

if(WIN32)
	# Win32 CMake find_package() searches <prefix>/CMake/
	set(CONFIG_PREFIX "CMake")
else()
	# UNIX CMake find_package() searches <prefix>/share/cmake
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

