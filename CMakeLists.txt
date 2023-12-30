cmake_minimum_required(VERSION 3.26)
project(nanojsonc C)

set(CMAKE_C_STANDARD 11)

add_library(nanojsonc src/object.c src/array.c)
target_compile_features(nanojsonc PRIVATE c_std_11)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

target_include_directories(nanojsonc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

# If you want to export the library as part of a package for VCPKG, use the following lines:
install(TARGETS nanojsonc
        EXPORT nanojsonc
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_package_config_file(build/config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/nanojsonc
        NO_SET_AND_CHECK_MACRO)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_DATADIR}/nanojsonc)

install(EXPORT nanojsonc
        FILE nanojsoncConfig.cmake
        NAMESPACE nanojsonc::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/nanojsonc
)

# Installs include directory (headers)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nanojsonc)

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.1 AND NOT WIN32) # sanitizer
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -fno-sanitize-recover=all")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
endif ()

if (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32) # werror
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -ansi")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ansi -pedantic")
endif ()

# Tests
option(BUILD_TESTS "Build Tests" ON)
if (BUILD_TESTS)
    enable_testing()
    include(CTest)

    add_subdirectory(test)
    add_subdirectory(example)
endif ()