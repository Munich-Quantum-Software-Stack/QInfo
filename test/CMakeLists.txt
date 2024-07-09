#-------------------------------------------------------------------------------
# Part of the MQSS Project, under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#-------------------------------------------------------------------------------

add_executable(InfoCreation create.c)
target_link_libraries(InfoCreation qinfo::qinfo)
add_test(InfoCreation InfoCreation)

add_executable(InfoAddDel adddel.c)
target_link_libraries(InfoAddDel qinfo::qinfo)
add_test(InfoAddDel InfoAddDel)

add_executable(InfoAddMany addmany.c)
target_link_libraries(InfoAddMany qinfo::qinfo)
add_test(InfoAddMany InfoAddMany)

add_executable(InfoAddDouble adddouble.c)
target_link_libraries(InfoAddDouble qinfo::qinfo)
add_test(InfoAddDouble InfoAddDouble)

add_executable(InfoValue value.c)
target_link_libraries(InfoValue qinfo::qinfo)
add_test(InfoValue InfoValue)
