# ------------------------------------------------------------------------------
# Part of the MQSS Project, under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# ------------------------------------------------------------------------------

if(QINFO_INSTALL)
  include(GNUInstallDirs)
  set(QINFO_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/cmake/qinfo")
  set(QINFO_INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/qinfo")
  set(QINFO_TARGETS_EXPORT_NAME "qinfo-targets")
  set(QINFO_CMAKE_CONFIG_TEMPLATE
      "${PROJECT_SOURCE_DIR}/cmake/qinfo-config.cmake.in")
  set(QINFO_CMAKE_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}")
  set(QINFO_CMAKE_VERSION_CONFIG_FILE
      "${QINFO_CMAKE_CONFIG_DIR}/qinfo-config-version.cmake")
  set(QINFO_CMAKE_PROJECT_CONFIG_FILE
      "${QINFO_CMAKE_CONFIG_DIR}/qinfo-config.cmake")
  set(QINFO_CMAKE_PROJECT_TARGETS_FILE
      "${QINFO_CMAKE_CONFIG_DIR}/qinfo-targets.cmake")
endif()

if(NOT TARGET project_warnings)
  # Use the warnings specified in CompilerWarnings.cmake
  add_library(project_warnings INTERFACE)

  # Standard compiler warnings
  include(${PROJECT_SOURCE_DIR}/cmake/CompilerWarnings.cmake)
  set_project_warnings(project_warnings)

  # Add alias
  add_library(qinfo::project_warnings ALIAS project_warnings)
endif()

if(NOT TARGET qinfo)
  add_library(qinfo qinfo.c ${QINFO_INCLUDE_BUILD_DIR}/qinfo.h)

  # build as shared lib by default
  if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(qinfo PUBLIC QINFO_STATIC_DEFINE)
  endif()

  # set include directories
  target_include_directories(
    qinfo PUBLIC $<BUILD_INTERFACE:${QINFO_INCLUDE_BUILD_DIR}>)

  # add warnings to the library
  target_link_libraries(qinfo PRIVATE qinfo::project_warnings)

  # set required C standard
  target_compile_features(qinfo PUBLIC c_std_11)

  # always include debug symbols (avoids common problems with LTO)
  target_compile_options(qinfo PUBLIC -g)

  # enable coverage collection options
  option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
  if(ENABLE_COVERAGE)
    target_compile_options(qinfo PUBLIC --coverage -fprofile-arcs
                                        -ftest-coverage -O0)
    target_link_libraries(qinfo PUBLIC gcov --coverage)
  endif()

  include(CheckCCompilerFlag)
  check_c_compiler_flag(-mtune=native HAS_MTUNE_NATIVE)
  if(HAS_MTUNE_NATIVE)
    target_compile_options(qinfo PRIVATE -mtune=native)
  endif()

  check_c_compiler_flag(-march=native HAS_MARCH_NATIVE)
  if(HAS_MARCH_NATIVE)
    target_compile_options(qinfo PRIVATE -march=native)
  endif()

  # set the version of the library
  set_target_properties(
    qinfo
    PROPERTIES VERSION ${PROJECT_VERSION}
               SOVERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
               PUBLIC_HEADER "${QINFO_INCLUDE_BUILD_DIR}/qinfo.h")

  # add alias
  add_library(qinfo::qinfo ALIAS qinfo)
endif()

# Installation instructions for the main library
if(QINFO_INSTALL)
  include(CMakePackageConfigHelpers)
  configure_package_config_file(
    ${QINFO_CMAKE_CONFIG_TEMPLATE} ${QINFO_CMAKE_PROJECT_CONFIG_FILE}
    INSTALL_DESTINATION ${QINFO_CONFIG_INSTALL_DIR}
    NO_SET_AND_CHECK_MACRO NO_CHECK_REQUIRED_COMPONENTS_MACRO)
  write_basic_package_version_file(
    ${QINFO_CMAKE_VERSION_CONFIG_FILE}
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMinorVersion)

  install(FILES ${QINFO_CMAKE_PROJECT_CONFIG_FILE}
                ${QINFO_CMAKE_VERSION_CONFIG_FILE}
          DESTINATION ${QINFO_CONFIG_INSTALL_DIR})

  install(
    TARGETS qinfo project_warnings
    EXPORT ${QINFO_TARGETS_EXPORT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT QInfo_Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT QInfo_Runtime
            NAMELINK_COMPONENT QInfo_Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT QInfo_Development
    PUBLIC_HEADER DESTINATION ${QINFO_INCLUDE_INSTALL_DIR})

  install(
    EXPORT ${QINFO_TARGETS_EXPORT_NAME}
    FILE ${QINFO_PROJECT_TARGETS_FILE}
    NAMESPACE qinfo::
    DESTINATION ${QINFO_CONFIG_INSTALL_DIR}
    COMPONENT QInfo_Development)

  install(FILES ${PROJECT_SOURCE_DIR}/cmake/CompilerWarnings.cmake
          DESTINATION ${QINFO_CONFIG_INSTALL_DIR})
endif()
