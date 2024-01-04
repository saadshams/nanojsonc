#set(CTEST_SOURCE_DIRECTORY "/src")
#set(CTEST_BINARY_DIRECTORY "/bin")
#
#set(ENV{CXXFLAGS} "--coverage")
#set(CTEST_CMAKE_GENERATOR "Ninja")
#set(CTEST_USE_LAUNCHERS 1)
#
#set(CTEST_COVERAGE_COMMAND "gcov")
#set(CTEST_MEMORYCHECK_COMMAND "valgrind")
##set(CTEST_MEMORYCHECK_TYPE"ThreadSanitizer")
#
#ctest_start("Continuous")
#ctest_configure()
#ctest_build()
#ctest_test()
#ctest_coverage()
#ctest_memcheck()
#ctest_submit()

add_executable(test_object test_object.c)
target_link_libraries(test_object PRIVATE nanojsonc)
add_test(NAME TestObject COMMAND test_object)

add_executable(test_array test_array.c)
target_link_libraries(test_array PRIVATE nanojsonc)
add_test(NAME TestArray COMMAND test_array)

