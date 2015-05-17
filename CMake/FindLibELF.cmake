#.rst:
# FindLibELF
# ----------
# 
# Find Executable Linkable Format library.
# 
# Result Variables
# ~~~~~~~~~~~~~~~~
# 
# This module defines the following variables:
# 
# ``LIBELF_FOUND``
#  ``TRUE`` if libelf was found on the current system.
# ``LIBELF_INCLUDE_DIR``
#  Directories containing the libelf headers.
# ``LIBELF_LIBRARY``
#  Libelf library to link against.
# ``LIBELF_HAVE_GELF``
#  ``TRUE`` if libelf has GElf interface.
# 

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

if(APPLE)
	# Fink
	list(APPEND LIBELF_PATHS "/sw")
	# DarwinPorts
	list(APPEND LIBELF_PATHS "/opt/local")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "(Solaris|SunOS)")
	# BlastWave
	list(APPEND LIBELF_PATHS "/opt/csw")
endif()

if(UNIX)
	list(APPEND LIBELF_PATHS "/usr")
	list(APPEND LIBELF_PATHS "/usr/local")
endif()

find_path(LIBELF_INCLUDE_DIR NAMES libelf.h PATHS ${LIBELF_PATHS} PATH_SUFFIXES include include/libelf)

find_library(LIBELF_LIBRARY NAMES elf PATHS ${LIBELF_PATHS} PATH_SUFFIXES lib )

if(LIBELF_INCLUDE_DIR AND EXISTS ${LIBELF_INCLUDE_DIR}/gelf.h)
	set(LIBELF_HAVE_GELF TRUE)
endif()

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(LibELF REQUIRED_VARS LIBELF_LIBRARY LIBELF_INCLUDE_DIR)

