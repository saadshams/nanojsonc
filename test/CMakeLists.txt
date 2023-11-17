add_executable(test_object test_object.c)
target_link_libraries(test_object PRIVATE nanojsonc)
target_include_directories(test_object PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
add_test(NAME TestObject COMMAND test_object)

add_executable(test_array test_array.c)
target_link_libraries(test_array PRIVATE nanojsonc)
target_include_directories(test_array PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
add_test(NAME TestArray COMMAND test_array)
