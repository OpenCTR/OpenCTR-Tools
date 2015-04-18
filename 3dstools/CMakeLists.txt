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

include(ExternalProject)

ExternalProject_Add(3dstools
	PREFIX ${CMAKE_CURRENT_BINARY_DIR}
	INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/install
	URL "https://github.com/devkitPro/3dstools/archive/v1.0.4.tar.gz"
	URL_HASH MD5=b6aa364b3ddbd34923cb6a230828f70f
	DOWNLOAD_NO_PROGRESS TRUE
	CONFIGURE_COMMAND 
		# Run autogen.sh in SOURCE_DIR
		COMMAND ${CMAKE_COMMAND} -E chdir <SOURCE_DIR> ${BASH_EXECUTABLE} ./autogen.sh &> <TMP_DIR>/3dstools-autoconf.log
		# Run configure in BUILD_DIR
		COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> &> <TMP_DIR>/3dstools-configure.log
	BUILD_COMMAND ${MAKE_EXECUTABLE} &> <TMP_DIR>/3dstools-make.log
	INSTALL_COMMAND ${MAKE_EXECUTABLE} install &> <TMP_DIR>/3dstools-install.log
)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "src" "tmp" "install")

install(
	DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/install/
	DESTINATION .
	USE_SOURCE_PERMISSIONS
	MESSAGE_NEVER
)

