cmake_minimum_required(VERSION 3.26)
project(nanojsonc C)

set(CMAKE_C_STANDARD 11)

add_library(nanojsonc src/object.c src/array.c)
target_compile_features(nanojsonc PRIVATE c_std_11)

target_include_directories(nanojsonc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# VCPKG
# install the target nanojsonc and create an export set nanojsonc, target sets used to import targets into other CMAKE project
# Archive static or dynamic library will be installed to the lib folder
# INCLUDES DESTINATION tells CMAKE, when other targets link to this target, the include directory should be added to their include directories
# all paths are relative to CMAKE_INSTALL_PREFIX /usr/local
# GNUInstallDirs gives you access to CMAKE_INSTALL_LIBDIR and CMAKE_INSTALL_INCLUDEDIR
#install(TARGETS nanojsonc
#        EXPORT nanojsonc
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # x64-osx/lib
#        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # x64-osx/include
#)
install(TARGETS nanojsonc
        EXPORT nanojsoncTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # x64-osx/lib
)
# removed INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # include

# config.cmake is the input template configuration file for the package that is being built.
# CMAKE_CURRENT_BINARY_DIR is the output file path to source directory/cmake-build-debug
# the configured file will be written to at this location
# datadir directory for read-only architecture-independent data files
# NO_SET_AND_CHECK_MACRO instructs not to set
configure_package_config_file(build/config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config.cmake # cmake-build-debug
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/nanojsonc NO_SET_AND_CHECK_MACRO) # x64-osx/share

# cmake-build/debug/nanojsonc-config.cmake and version.cmake
# install above two files to share directory
#install(FILES
#            ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config.cmake # cmake-build-debug
#            ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config-version.cmake # cmake_build-debug
#        DESTINATION
#            ${CMAKE_INSTALL_DATADIR}/nanojsonc) # x64-osx/share


# EXPORT nanojsonc, name of the export set that was created by the install(TARGETS ... EXPORT ...) command
# FILE, name of the output file generated
# NAMESPACE nanojsonc:: this option means that for imported targets, they will have nanojsonc added as prefix
# it helps distinguish these imported targets from other targets in the project
# the path where the file will be installed,
# changed from CMAKE_INSTALL_LIBDIR to CMAKE_INSTALL_DATADIR, added Targets suffix
install(EXPORT nanojsoncTargets
        FILE nanojsoncTargets.cmake
        NAMESPACE nanojsoncTargets::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/nanojsonc # x64-osx/lib
)
#install(EXPORT nanojsoncTargets
#        FILE nanojsoncTargets.cmake
#        NAMESPACE nanojsoncTargets::
#        DESTINATION ${CMAKE_CURRENT_LIST_DIR}/cmake/nanojsoncTargets
#)

# Install headers
# DIRECTORY installing a whole directory
# source directory/include to destination include directory under the folder nanojsonc
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ # root
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nanojsonc) # x64-osx/include

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