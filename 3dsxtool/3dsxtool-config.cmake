#.rst:
# 3dsxtool-config
# ---------------
#
# :command:`find_package` interface to `3dsxtool`.
#
# Finds the ``3dsxtool`` executable, and creates the 
# :command:`ctr_convert_binary` command.

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

include(CMakeParseArguments)

# Make sure CMake 3.0.0 or later is used.
if(CMAKE_VERSION VERSION_LESS 3.0.0)
    set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
    set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
        "${CMAKE_FIND_PACKAGE_NAME} requires CMake 3.0.0 or later.")
    return()
endif()

#[========================================[.rst:
.. command:: ctr_convert_binary

 Add a custom rule to convert ELF files into 3DSX after compiling.

 ::

   ctr_convert_binary(<TARGET> [OUTPUT <file>])

 ``TARGET``
  Target created with :command:`add_executable`. Must be an ARM 
  ELF Executable. Target will be converted from ELF to 3DSX.
 ``OUTPUT``
  By default, the output file is named ``<TARGET>.3dsx``. If 
  ``OUTPUT`` is given, the output file will be named ``<file>``.
#]========================================]
function(CTR_CONVERT_BINARY TGT)
    set(BOOLEANS)
    set(SINGLES OUTPUT)
    set(MULTIS)

    cmake_parse_arguments(3DSXTOOL "${BOOLEANS}" "${SINGLES}" "${MULTIS}"
        ${ARGN})

    if(NOT TARGET ${TGT})
        message(SEND_ERROR
            "Error: CTR_CONVERT_BINART target ${TGT} is not a target.")
        return()
    endif()

    if(NOT 3DSXTOOL_OUTPUT)
        set(3DSXTOOL_OUTPUT "$<TARGET_FILE_DIR:${TGT}>/${TGT}.3dsx")
    endif()

    # Working directory
    get_filename_component(BUILD_DIR "$<TARGET_FILE:${TGT}>" DIRECTORY)

    # Location of 3dsxtool.
    get_property(3DSXTOOL_EXECUTABLE TARGET CTR::3dsxtool PROPERTY LOCATION)

    add_custom_command(TARGET ${TGT}
        POST_BUILD
        COMMAND ${3DSXTOOL_EXECUTABLE} $<TARGET_FILE:${TGT}> ${3DSXTOOL_OUTPUT}
        WORKING_DIRECTORY ${BUILD_DIR}
        COMMENT "Generating ${TGT}.3dsx"
    )
endfunction()

# Include the 3dsxtool targets.
include(${CMAKE_CURRENT_LIST_DIR}/3dsxtool-targets.cmake
    OPTIONAL RESULT_VARIABLE ret)
if(NOT ret)
    set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
    set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
        "3dsxtool-targets.cmake not found.")
    return()
endif()

# Make sure CTR::3dsxtool was exported.
if(NOT TARGET CTR::3dsxtool)
    set(${CMAKE_FIND_PACKAGE_NAME}_FOUND FALSE)
    set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE
        "CTR::3dsxtool target not found.")
    return()
endif()
