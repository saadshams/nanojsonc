cmake_minimum_required(VERSION 3.26)
project(nanojsonc C)

set(CMAKE_C_STANDARD 11)

add_library(nanojsonc src/object.c src/array.c)
target_compile_features(nanojsonc PRIVATE c_std_11)

target_include_directories(nanojsonc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
#target_include_directories(${PROJECT_NAME} INTERFACE $<INSTALL_INTERFACE:include>) # failing array.c, object.c

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# VCPKG
# CONFIG START

configure_package_config_file(build/config.cmake # config file ${PROJECT_NAME}
        ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config.cmake # cmake-build-debug
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/nanojsonc NO_SET_AND_CHECK_MACRO) # share

write_basic_package_version_file( # version file
        ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config-version.cmake
        VERSION 1.0.0
        COMPATIBILITY SameMajorVersion)

install(FILES # install config and version to share directory
            ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config.cmake # cmake-build-debug
            ${CMAKE_CURRENT_BINARY_DIR}/nanojsonc-config-version.cmake # cmake_build-debug
        DESTINATION
            ${CMAKE_INSTALL_DATADIR}/nanojsonc) # share

set(ConfigPackageLocation ${CMAKE_INSTALL_DATADIR}/nanojsonc)

# TARGET INSTALLATION
# install the target nanojsonc and create an export set nanojsonc, target sets used to import targets into other CMAKE project

#install(TARGETS nanojsonc EXPORT nanojsonc-targets) # create target set
#install(EXPORT nanojsonc-targets DESTINATION ${CMAKE_INSTALL_DATADIR}/nanojsonc) # export target set

# NAMESPACE nanojsonc::

#install(TARGETS ${PROJECT_NAME}
#        EXPORT ${PROJECT_NAME}Config
#        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # x64-osx/lib
#        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # x64-osx/lib
#        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # x64-osx/include
#)

#install(EXPORT ${PROJECT_NAME}Config
#        FILE ${PROJECT_NAME}Config.cmake
#        DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME} # share
#)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nanojsonc) # headers

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.1 AND NOT WIN32) # sanitizer
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -fno-sanitize-recover=all")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
endif ()

if (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32) # werror
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -ansi")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ansi -pedantic")
endif ()

option(BUILD_TESTS "Build Tests" ON) # Tests
if (BUILD_TESTS)
    enable_testing()
    include(CTest)

    add_subdirectory(test)
    add_subdirectory(example)
endif ()
