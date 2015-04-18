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

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

add_executable(3dsxtool
	3dsxtool.cpp
)

install(TARGETS 3dsxtool 
	EXPORT 3dsxtool
	RUNTIME DESTINATION bin
)

if(WIN32)
	# Win32 CMake find_package() searches <prefix>/CMake/
	install(EXPORT 3dsxtool
		DESTINATION CMake
		FILE ctrxtool.cmake
	)
else()
	# UNIX CMake find_package() searches <prefix>/lib/cmake
	install(EXPORT 3dsxtool
		DESTINATION lib/cmake
		FILE 3dsxtool.cmake
	)

endif()
