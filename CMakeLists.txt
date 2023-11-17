cmake_minimum_required(VERSION 3.26)
project(nanojsonc C)

include(GNUInstallDirs) # Include GNUInstallDirs to set installation directories

set(CMAKE_C_STANDARD 11)

add_library(nanojsonc src/object.c src/array.c)
target_compile_features(nanojsonc PRIVATE c_std_11)

target_include_directories(nanojsonc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

# Installation rules
install(
        TARGETS nanojsonc
        EXPORT nanojsoncTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# installs header
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nanojsonc)

include(CMakePackageConfigHelpers)
configure_file(cmake/nanoJSONcConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/nanoJSONcConfig.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/nanoJSONcConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nanojsonc)

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.1 AND NOT WIN32) # sanitizer
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -fno-sanitize-recover=all")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
endif ()

if (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32) # werror
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif ()

option(NANOJSONC_BUILD_TESTS "Build tests" ON) # tests
if (NANOJSONC_BUILD_TESTS)
    enable_testing()
    include(CTest)
    set(CTEST_OUTPUT_ON_FAILURE ON)

    add_subdirectory(test)
    add_subdirectory(example)
endif ()
