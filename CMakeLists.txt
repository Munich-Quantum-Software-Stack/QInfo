# set required cmake version
cmake_minimum_required(VERSION 3.19...3.30)

project(
  qinfo
  LANGUAGES C
  VERSION 0.2.0
  DESCRIPTION "QInfo Library")

# make scripts available to cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# check if this is the master project or used via add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(QINFO_MASTER_PROJECT ON)
else()
  set(QINFO_MASTER_PROJECT OFF)
endif()

option(QINFO_INSTALL "Generate installation instructions for QInfo" ${QINFO_MASTER_PROJECT})
option(BUILD_QINFO_TESTS "Also build tests for the QInfo project" ${QINFO_MASTER_PROJECT})

# enable organization of targets into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui, ccmake
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel"
                                               "RelWithDebInfo")
endif()

# Generate compile_commands.json to make it easier to work with clang-based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS
    ON
    CACHE BOOL "Export compile commands" FORCE)

# Require C standard
set_property(GLOBAL PROPERTY CMAKE_C_STANDARD_REQUIRED ON)
set_property(GLOBAL PROPERTY C_EXTENSIONS OFF)

# set the include directory for the build tree
set(QINFO_INCLUDE_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/qinfo")

# add main library code
add_subdirectory(src)

# add test code
if(BUILD_QINFO_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
