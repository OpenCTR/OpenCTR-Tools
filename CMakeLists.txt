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

cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug" CACHE INTERNAL "Values: Debug, Release, RelWithDebInfo, MinSizeRel")
endif()

project(OpenCTR-Tools NONE)

include(CMakeProjectSettings)
include(CMakeProjectVersion)

# Enable 3dsxtool by default
option(ENABLE_3DSXTOOL "Enable/Disable 3dsxtool" ON)
# Enable artool by default
option(ENABLE_ARTOOL "Enable/Disable artool" ON)
# Disable ctrxtool by default
option(ENABLE_CTRXTOOL "Enable/Disable ctrxtool" OFF)
# Disable smdhtool by default
option(ENABLE_SMDHTOOL "Enable/Disable smdhtool" OFF)

if(ENABLE_3DSXTOOL)
	add_subdirectory(3dsxtool)
endif()

if(ENABLE_ARTOOL)
	add_subdirectory(artool)
endif()

if(ENABLE_CTRXTOOL)
	add_subdirectory(ctrxtool)
endif()

if(ENABLE_SMDHTOOL)
	add_subdirectory(smdhtool)
endif()

