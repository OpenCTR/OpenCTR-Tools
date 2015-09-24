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

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
cmake_policy(SET CMP0048 NEW)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(CMAKE_BUILD_TYPE_INIT "Debug")

project(OpenCTR-Tools)

add_subdirectory(3dsxtool)
add_subdirectory(artool)
