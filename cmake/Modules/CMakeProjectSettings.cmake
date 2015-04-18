#.rst:
# CMakeProjectSettings
# --------------------
# 
# Project settings.
# 

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

# Require GNU Make for building Autotools projects
find_program(MAKE_EXECUTABLE make)
if(NOT MAKE_EXECUTABLE)
	message(SEND_ERROR "GNU Make not found")
endif()

# Require POSIX Shell for running configure scripts
find_program(BASH_EXECUTABLE NAMES bash sh)
if(NOT BASH_EXECUTABLE)
	message(SEND_ERROR "POSIX Shell (bash) not found")
endif()

