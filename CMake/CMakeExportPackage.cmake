#.rst:
# CMakeExportPackage
# ------------------
# 
# 
# Helpers functions for creating config files that can be included by other
# projects to find and use a package.
#
# Adds the :command:`cmake_export_package()` commands.
#
# cmake_export_package
# ^^^^^^^^^^^^^^^^^^^^
#
# .. command:: cmake_export_package
#
#  Create config files for a project
# 
#  ::
#
#    cmake_export_package(
#      INPUT_FILE <input>
#      OUTPUT_FILE <output>
#      INSTALL_DESTINATION <path>
#      [VERSION_FILE <output>]
#      [VERSION <major.minor.patch>]
#      [COMPATIBILITY <AnyNewerVersion|SameMajorVersion|ExactVersion>]
#      [PATH_VARS <var1> <var2> ... <varN>]
#      [NO_SET_AND_CHECK_MACRO]
#      [NO_CHECK_REQUIRED_COMPONENTS_MACRO]
#      [INSTALL_PREFIX <path>]
#    )
# 
#  ``cmake_export_package`` makes package configuration files relocatable by 
#  avoiding use of any hardcoded paths. 
# 
#  To use ``cmake_export_package`` correctly, follow these guidelines:
#  
#  1. insert a line containing only the string ``@PACKAGE_INIT@``
#  2. Use of any ``PACKAGE\_*XXX*`` variables (where ``*XXX*`` is a variable 
#     which was listed in ``PATH_VARS``) must come after ``@PACKAGE_INIT@``
#  3. Use ``cmake_export_package`` instead of the normal 
#     :command:`configure_file()`
# 
#  ``INPUT_FILE <input>`` and ``OUTPUT_FILE <output>`` give the input and 
#  output files, respectively.
# 
#  The ``<path>`` given to ``INSTALL_DESTINATION`` must be the destination 
#  where the ``FooConfig.cmake`` file will be installed to. This path can 
#  either be absolute, or relative to the ``INSTALL_PREFIX`` path.
# 
#  The variables ``<var1>`` to ``<varN>`` given as ``PATH_VARS`` are the
#  variables which contain install destinations. For each of them the macro 
#  will create a helper variable ``PACKAGE_<var...>``. This works both for
#  relative and also for absolute locations, as long as the absolute 
#  location is a subdirectory of ``INSTALL_PREFIX``.
#
#  If the ``INSTALL_PREFIX`` argument is passed, this is used as base path 
#  to calculate all the relative paths. The ``<path>`` argument must be an 
#  absolute path. If this argument is not passed, the 
#  :variable:`CMAKE_INSTALL_PREFIX` variable will be used instead.
#  
#  If ``VERSION_FILE <output>`` is given, ``cmake_export_package`` will 
#  also generate a version file.
# 
#  When ``VERSION_FILE`` is enabled, ``VERSION <major.minor.patch>`` is 
#  the version number of the project to be installed. If ``VERSION`` is 
#  not given, the :variable:`PROJECT_VERSION` variable is used. If this 
#  hasn't been set, an error is given.
# 
#  ``COMPATIBILITY_MODE <mode>`` helps determine if the installed package 
#  version is usable. ``AnyNewerVersion`` means that the installed package 
#  version will be considered compatible if it is newer or exactly the same 
#  as the requested version. ``SameMajorVersion`` means that the package is 
#  considered compatible *only* if the requested version major number is the 
#  same as the installed package version. ``ExactVersion`` means that the 
#  package is only considered compatible if the requested version matches 
#  exactly the installed package version number. If ``COMPATIBILITY_MODE`` 
#  is not given, the default mode is ``AnyNewerVersion``.
# 
#  By default ``cmake_export_package`` also generates two helper 
#  macros, ``set_and_check()`` and ``check_required_components()`` into the
#  output file.
#
#  ``set_and_check()`` should be used instead of the normal ``set()`` command 
#  for setting directories and file locations. Additionally to setting the 
#  variable it also checks that the referenced file or directory actually 
#  exists and fails with a ``FATAL_ERROR`` otherwise. This makes sure that 
#  the output file does not contain wrong references. When the 
#  ``NO_SET_AND_CHECK_MACRO`` option is given, this macro will not be added.
#
#  ``check_required_components(<package_name>)`` should be called at the end 
#  of the input file if the package supports components. This macro checks 
#  whether all requested, non-optional components have been found, and if 
#  this is not the case, sets the ``XXX_FOUND`` variable to ``FALSE``, so that 
#  the package is considered to be not found. It does that by testing the
#  ``XXX_<Component>_FOUND`` variables for all requested required components.
#  When the ``NO_CHECK_REQUIRED_COMPONENTS_MACRO`` option is given, this 
#  macro will not be added.
#
# Example
# ^^^^^^^
#
# Example using :command:`cmake_export_package`:
#
# ``CMakeLists.txt``:
#
# .. code-block:: cmake
#
#    set(INCLUDE_INSTALL_DIR include)
#    set(LIB_INSTALL_DIR lib)
#    set(SYSCONFIG_INSTALL_DIR etc/foo)
#    
#    ...
#    
#    include(CMakePackageConfigHelpers)
#    
#    cmake_export_package(
#      INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/FooConfig.cmake.in
#      OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/FooConfig.cmake
#      VERSION_FILE ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#      INSTALL_DESTINATION ${LIB_INSTALL_DIR}/Foo/cmake
#      PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR
#      VERSION 1.2.3
#      COMPATIBILITY SameMajorVersion
#    )
#    
#    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/FooConfig.cmake
#                  ${CMAKE_CURRENT_BINARY_DIR}/FooConfigVersion.cmake
#            DESTINATION ${LIB_INSTALL_DIR}/Foo/cmake
#    )
#
# ``FooConfig.cmake.in``:
#
# .. code-block:: cmake
#
#    set(FOO_VERSION x.x.x)
# 
#    ...
# 
#    @PACKAGE_INIT@
# 
#    ...
# 
#    set_and_check(FOO_INCLUDE_DIR "@PACKAGE_INCLUDE_INSTALL_DIR@")
#    set_and_check(FOO_SYSCONFIG_DIR "@PACKAGE_SYSCONFIG_INSTALL_DIR@")
#
#    check_required_components(Foo)


###############################################################################
# Copyright (C) 2015 OpenCTR Contributors
# 
# This file is part of OpenCTR.
# 
# OpenCTR is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
# 
# OpenCTR is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
# 
###############################################################################

include(CMakeParseArguments)

function(CMAKE_EXPORT_PACKAGE)
	set(BOOLEAN_OPTIONS NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO)
	set(SINGLE_OPTIONS INPUT_FILE OUTPUT_FILE INSTALL_DESTINATION INSTALL_PREFIX VERSION_FILE VERSION COMPATIBILITY)
	set(MULTIPLE_OPTIONS PATH_VARS)

	cmake_parse_arguments(PKG "${BOOLEAN_OPTIONS}" "${SINGLE_OPTIONS}" "${MULTIPLE}" ${ARGN})

	if(PKG_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "Unknown keywords given to CMAKE_EXPORT_PACKAGE(): \"${PKG_UNPARSED_ARGUMENTS}\"")
	elseif(NOT PKG_INPUT_FILE)
		message(FATAL_ERROR "No INPUT_FILE given to CMAKE_EXPORT_PACKAGE()")
	elseif(NOT PKG_OUTPUT_FILE)
		message(FATAL_ERROR "No OUTPUT_FILE given to CMAKE_EXPORT_PACKAGE()")
	elseif(NOT PKG_INSTALL_DESTINATION)
		message(FATAL_ERROR "No INSTALL_DESTINATION given to CMAKE_EXPORT_PACKAGE()")
	endif()

	if(PKG_VERSION_FILE)
		if(NOT PKG_VERSION)
			message(FATAL_ERROR "No VERSION given to CMAKE_EXPORT_PACKAGE()")
		endif()

		if(NOT PKG_COMPATIBILITY)
			set(PKG_COMPATIBILITY "AnyNewerVersion")
		endif()

		set(versionTemplateFile "${CMAKE_ROOT}/Modules/BasicConfigVersion-${PKG_COMPATIBILITY}.cmake.in")
		if(NOT EXISTS "${versionTemplateFile}")
			message(FATAL_ERROR "Bad COMPATIBILITY value given to CMAKE_EXPORT_PACKAGE(): \"${PKG_COMPATIBILITY}\"")
		endif()

		configure_file("${versionTemplateFile}" "${PKG_VERSION_FILE}" @ONLY)
	endif()

	if(DEFINED PKG_INSTALL_PREFIX)
		if(IS_ABSOLUTE "${PKG_INSTALL_PREFIX}")
			set(installPrefix "${PKG_INSTALL_PREFIX}")
		else()
			message(FATAL_ERROR "INSTALL_PREFIX must be an absolute path")
		endif()
	else()
		set(installPrefix "${CMAKE_INSTALL_PREFIX}")
	endif()

	if(IS_ABSOLUTE "${PKG_INSTALL_DESTINATION}")
		set(absInstallDir "${PKG_INSTALL_DESTINATION}")
	else()
		set(absInstallDir "${installPrefix}/${PKG_INSTALL_DESTINATION}")
	endif()

	file(RELATIVE_PATH PACKAGE_RELATIVE_PATH "${absInstallDir}" "${installPrefix}")

	foreach(var ${PKG_PATH_VARS})
		if(NOT DEFINED ${var})
			message(FATAL_ERROR "Variable ${var} does not exist")
		else()
			if(IS_ABSOLUTE "${${var}}")
				string(REPLACE "${installPrefix}" "\${PACKAGE_PREFIX_DIR}" PACKAGE_${var} "${${var}}")
			else()
				set(PACKAGE_${var} "\${PACKAGE_PREFIX_DIR}/${${var}}")
			endif()
		endif()
	endforeach()

	get_filename_component(inputFileName "${PKG_INPUT_FILE}" NAME)

	##########################################################################

	set(PACKAGE_INIT "
####### Expanded from @PACKAGE_INIT@ by CMAKE_EXPORT_PACKAGE() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was ${inputFileName}                            ########

get_filename_component(PACKAGE_PREFIX_DIR \"\${CMAKE_CURRENT_LIST_DIR}/${PACKAGE_RELATIVE_PATH}\" ABSOLUTE)
")

	##########################################################################

	# Handle "/usr move" symlinks created by some Linux distros.
	if("${absInstallDir}" MATCHES "^(/usr)?/lib(64)?/.+")
		set(PACKAGE_INIT "${PACKAGE_INIT}
# Use original install prefix when loaded through a \"/usr move\"
# cross-prefix symbolic link such as /lib -> /usr/lib.
get_filename_component(_realCurr \"\${CMAKE_CURRENT_LIST_DIR}\" REALPATH)
get_filename_component(_realOrig \"${absInstallDir}\" REALPATH)
if(_realCurr STREQUAL _realOrig)
  set(PACKAGE_PREFIX_DIR \"${installPrefix}\")
endif()
unset(_realOrig)
unset(_realCurr)
")
	endif()

	##########################################################################

	if(NOT PKG_NO_SET_AND_CHECK_MACRO)
		set(PACKAGE_INIT "${PACKAGE_INIT}
macro(set_and_check _var _file)
  set(\${_var} \"\${_file}\")
  if(NOT EXISTS \"\${_file}\")
    message(FATAL_ERROR \"File or directory \${_file} referenced by variable \${_var} does not exist !\")
  endif()
endmacro()
")
	endif()

	##########################################################################

	if(NOT PKG_NO_CHECK_REQUIRED_COMPONENTS_MACRO)
		set(PACKAGE_INIT "${PACKAGE_INIT}
macro(check_required_components _NAME)
  foreach(comp \${\${_NAME}_FIND_COMPONENTS})
    if(NOT \${_NAME}_\${comp}_FOUND)
      if(\${_NAME}_FIND_REQUIRED_\${comp})
        set(\${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()
")
	endif()

	##########################################################################

	configure_file("${PKG_INPUT_FILE}" "${PKG_OUTPUT_FILE}" @ONLY)

endfunction()
