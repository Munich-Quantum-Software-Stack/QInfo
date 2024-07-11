# ------------------------------------------------------------------------------
# Part of the MQSS Project, under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# ------------------------------------------------------------------------------

if(TARGET qinfo_test)
  return()
endif()

# add CXX language support
enable_language(CXX)

# create an executable in which the tests will be stored
add_executable(qinfo_test test_qinfo.cpp)

# link the Google test infrastructure and a default main function to the test
# executable.
target_link_libraries(qinfo_test PRIVATE gtest_main qinfo::qinfo
                                         qinfo::project_warnings)

# discover tests
gtest_discover_tests(
  qinfo_test
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
             DISCOVERY_TIMEOUT 60)
