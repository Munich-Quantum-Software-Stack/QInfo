cmake_minimum_required(VERSION 3.9)

project(
  qinfo
  VERSION 0.1
  DESCRIPTION "QInfo Library")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      ${CMAKE_CURRENT_SOURCE_DIR}/../install
      CACHE PATH "..." FORCE)
endif()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/cmake")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}/cmake")

include(GNUInstallDirs)
include(CTest)

add_compile_options("-g")

add_library(qinfo SHARED src/qinfo.c)

set_target_properties(
  qinfo
  PROPERTIES VERSION ${PROJECT_VERSION}
             SOVERSION ${PROJECT_VERSION_MAJOR}
             PUBLIC_HEADER include/qinfo.h)

# configure_file(qinfo.pc.in qinfo.pc @ONLY)

# target_include_directories(qinfo PUBLIC include)

target_include_directories(qinfo PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                        $<INSTALL_INTERFACE:include>)

install(
  TARGETS qinfo
  EXPORT qinfoConfig
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

export(
  TARGETS qinfo
  NAMESPACE qinfo::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/qinfoConfig.cmake")

install(
  EXPORT qinfoConfig
  FILE qinfoConfig.cmake
  DESTINATION "${CMAKE_INSTALL_PREFIX}/cmake"
  NAMESPACE qinfo::)

# install(FILES ${CMAKE_BINARY_DIR}/mylib.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

add_executable(InfoCreation testing/create.c)
target_link_libraries(InfoCreation qinfo)
add_test(InfoCreation InfoCreation)

add_executable(InfoAddDel testing/adddel.c)
target_link_libraries(InfoAddDel qinfo)
add_test(InfoAddDel InfoAddDel)

add_executable(InfoAddMany testing/addmany.c)
target_link_libraries(InfoAddMany qinfo)
add_test(InfoAddMany InfoAddMany)

add_executable(InfoAddDouble testing/adddouble.c)
target_link_libraries(InfoAddDouble qinfo)
add_test(InfoAddDouble InfoAddDouble)

add_executable(InfoValue testing/value.c)
target_link_libraries(InfoValue qinfo)
add_test(InfoValue InfoValue)
