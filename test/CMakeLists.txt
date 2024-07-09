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
