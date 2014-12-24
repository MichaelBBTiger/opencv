# Based on https://github.com/rpavlik/cmake-modules/blob/master/CppcheckTargets.cmake
#      and https://github.com/rpavlik/cmake-modules/blob/master/Findcppcheck.cmake
#
# Original Author:
# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu> <abiryan@ryand.net>
# http://academic.cleardefinition.com
# Iowa State University HCI Graduate Program/VRAC
#
# Copyright Iowa State University 2009-2010.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

if(ENABLE_CPPCHECK)
  include(CMakeParseArguments)

  file(TO_CMAKE_PATH "${CPPCHECK_ROOT_DIR}" CPPCHECK_ROOT_DIR)
  set(CPPCHECK_ROOT_DIR "${CPPCHECK_ROOT_DIR}" CACHE PATH "Path to search for cppcheck")

  # cppcheck app bundles on Mac OS X are GUI, we want command line only
  set(_oldappbundlesetting ${CMAKE_FIND_APPBUNDLE})
  set(CMAKE_FIND_APPBUNDLE NEVER)

  if(CPPCHECK_EXECUTABLE AND NOT EXISTS "${CPPCHECK_EXECUTABLE}")
    set(CPPCHECK_EXECUTABLE "notfound" CACHE PATH FORCE "")
  endif()

  # If we have a custom path, look there first.
  if(CPPCHECK_ROOT_DIR)
    find_program(CPPCHECK_EXECUTABLE
      NAMES cppcheck cli
      PATHS "${CPPCHECK_ROOT_DIR}"
      PATH_SUFFIXES cli
      NO_DEFAULT_PATH)
  endif()

  find_program(CPPCHECK_EXECUTABLE NAMES cppcheck)

  # Restore original setting for appbundle finding
  set(CMAKE_FIND_APPBUNDLE ${_oldappbundlesetting})

  # Set dummy test file
  set(_cppcheckdummyfile "${CMAKE_SOURCE_DIR}/cmake/checks/winrttest.cpp")
  if(NOT EXISTS "${_cppcheckdummyfile}")
    message(FATAL_ERROR "Missing file ${_cppcheckdummyfile}")
  endif()

  function(_cppcheck_test_arg _resultvar _arg)
    if(NOT CPPCHECK_EXECUTABLE)
      set(${_resultvar} NO PARENT_SCOPE)
      return()
    endif()
    execute_process(COMMAND "${CPPCHECK_EXECUTABLE}" "${_arg}" --quiet "${_cppcheckdummyfile}"
      RESULT_VARIABLE _cppcheck_result
      OUTPUT_QUIET
      ERROR_QUIET
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
    if(_cppcheck_result EQUAL 0)
      set(${_resultvar} YES PARENT_SCOPE)
    else()
      set(${_resultvar} NO PARENT_SCOPE)
    endif()
  endfunction()

  function(_cppcheck_set_arg_var _argvar _arg)
    if("${${_argvar}}" STREQUAL "")
      _cppcheck_test_arg(_cppcheck_arg "${_arg}")
      if(_cppcheck_arg)
        set(${_argvar} "${_arg}" PARENT_SCOPE)
      endif()
    endif()
  endfunction()

  if(CPPCHECK_EXECUTABLE)
    # Check for the two types of command line arguments by just trying them
    _cppcheck_set_arg_var(CPPCHECK_STYLE_ARG "--enable=style")
    _cppcheck_set_arg_var(CPPCHECK_STYLE_ARG "--style")

    if("${CPPCHECK_STYLE_ARG}" STREQUAL "--enable=style")
      _cppcheck_set_arg_var(CPPCHECK_UNUSEDFUNC_ARG "--enable=unusedFunction")
      _cppcheck_set_arg_var(CPPCHECK_INFORMATION_ARG "--enable=information")
      _cppcheck_set_arg_var(CPPCHECK_MISSINGINCLUDE_ARG "--enable=missingInclude")
      _cppcheck_set_arg_var(CPPCHECK_POSIX_ARG "--enable=posix")
      _cppcheck_set_arg_var(CPPCHECK_POSSIBLEERROR_ARG "--enable=possibleError")
      _cppcheck_set_arg_var(CPPCHECK_POSSIBLEERROR_ARG "--enable=all")
      _cppcheck_set_arg_var(CPPCHECK_PERFORMANCE_ARG "--enable=performance")
      _cppcheck_set_arg_var(CPPCHECK_PORTABILITY_ARG "--enable=portability")
      if(MSVC)
        set(CPPCHECK_TEMPLATE_ARG "--template" "vs")
        set(CPPCHECK_FAIL_REGULAR_EXPRESSION "[(]error[)]")
        set(CPPCHECK_WARN_REGULAR_EXPRESSION "[(]style[)]")
      elseif(CMAKE_COMPILER_IS_GNUCXX)
        set(CPPCHECK_TEMPLATE_ARG "--template" "gcc")
        set(CPPCHECK_FAIL_REGULAR_EXPRESSION " error: ")
        set(CPPCHECK_WARN_REGULAR_EXPRESSION " style: ")
      else()
        set(CPPCHECK_TEMPLATE_ARG "--template" "gcc")
        set(CPPCHECK_FAIL_REGULAR_EXPRESSION " error: ")
        set(CPPCHECK_WARN_REGULAR_EXPRESSION " style: ")
      endif()
    elseif("${CPPCHECK_STYLE_ARG}" STREQUAL "--style")
      # Old arguments
      _cppcheck_set_arg_var(CPPCHECK_UNUSEDFUNC_ARG "--unused-functions")
      _cppcheck_set_arg_var(CPPCHECK_POSSIBLEERROR_ARG "--all")
      set(CPPCHECK_FAIL_REGULAR_EXPRESSION "error:")
      set(CPPCHECK_WARN_REGULAR_EXPRESSION "[(]style[)]")
    else()
      # No idea - some other issue must be getting in the way
      message(WARNING "Can't detect whether CPPCHECK wants new or old-style arguments!")
    endif()

    set(CPPCHECK_QUIET_ARG "--quiet")
    set(CPPCHECK_FORCE_ARG "--force")
    set(CPPCHECK_INCLUDEPATH_ARG "-I")
  endif()

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(cppcheck
    DEFAULT_MSG
    CPPCHECK_EXECUTABLE
    CPPCHECK_POSSIBLEERROR_ARG
    CPPCHECK_UNUSEDFUNC_ARG
    CPPCHECK_STYLE_ARG
    CPPCHECK_INCLUDEPATH_ARG
    CPPCHECK_QUIET_ARG)

  if(CPPCHECK_FOUND OR CPPCHECK_MARK_AS_ADVANCED)
    mark_as_advanced(CPPCHECK_ROOT_DIR)
  endif()

  mark_as_advanced(CPPCHECK_EXECUTABLE)
endif()

if(NOT CPPCHECK_FOUND)
  function(ocv_cppcheck TARGET_NAME)
    # nothing
  endfunction()
else()
  function(ocv_cppcheck TARGET_NAME)
    set(options "FORCE" "FAIL_ON_WARNINGS")
    set(oneValueArgs "PARALLEL_LEVEL" "ENABLE")
    set(multiValueArgs "INPUT" "ARGS" "LABELS")
    cmake_parse_arguments(CPPCHECK "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    list(APPEND CPPCHECK_ARGS "${CPPCHECK_QUIET_ARG}")
    if(CPPCHECK_FORCE)
      list(APPEND CPPCHECK_ARGS "${CPPCHECK_FORCE_ARG}")
    endif()
    if(CPPCHECK_PARALLEL_LEVEL)
      list(APPEND CPPCHECK_ARGS "-j${CPPCHECK_PARALLEL_LEVEL}")
    endif()
    foreach(item ${CPPCHECK_ENABLE})
      list(APPEND CPPCHECK_ARGS "${CPPCHECK_${item}_ARG}")
    endforeach()

    if(CPPCHECK_FAIL_ON_WARNINGS)
      list(APPEND CPPCHECK_FAIL_REGULAR_EXPRESSION ${CPPCHECK_WARN_REGULAR_EXPRESSION})
    endif()

    get_directory_property(DIR_INCLUDES DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" INCLUDE_DIRECTORIES)
    get_target_property(TARGET_INCLUDES "${TARGET_NAME}" INCLUDE_DIRECTORIES)
    foreach(_include ${DIR_INCLUDES} ${TARGET_INCLUDES})
      list(APPEND CPPCHECK_INCLUDES "${CPPCHECK_INCLUDEPATH_ARG}\"${_include}\"")
    endforeach()

    if(NOT CPPCHECK_INPUT)
      get_target_property(TARGET_SOURCES "${TARGET_NAME}" SOURCES)
      foreach(src_file ${TARGET_SOURCES})
        get_filename_component(src_file_loc "${src_file}" ABSOLUTE)
        if(EXISTS "${src_file_loc}")
          list(APPEND CPPCHECK_INPUT "${src_file_loc}")
        endif()
      endforeach()
    endif()

    add_test(NAME "${TARGET_NAME}_cppcheck"
      COMMAND "${CPPCHECK_EXECUTABLE}" ${CPPCHECK_TEMPLATE_ARG} ${CPPCHECK_ARGS} ${CPPCHECK_INCLUDES} ${CPPCHECK_INPUT})

    set_tests_properties("${TARGET_NAME}_cppcheck" PROPERTIES
      FAIL_REGULAR_EXPRESSION "${CPPCHECK_FAIL_REGULAR_EXPRESSION}"
      LABELS "${CPPCHECK_LABELS}"
      WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endfunction()
endif()
