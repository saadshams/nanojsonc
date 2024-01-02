cmake_minimum_required(VERSION 3.26)
project(nanojsonc C)

set(CMAKE_C_STANDARD 11)

add_library(nanojsonc src/object.c src/array.c)
target_compile_features(nanojsonc PRIVATE c_std_11)

target_include_directories(nanojsonc PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

## Config
#configure_package_config_file(build/config.cmake
#        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake # cmake-build-debug/
#        INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME} NO_SET_AND_CHECK_MACRO) # share/
#
## Version
#write_basic_package_version_file(
#        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
#        VERSION 1.0.0
#        COMPATIBILITY SameMajorVersion)
#
## Install Config & Version
#install(FILES
#            ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake # cmake-build-debug/
#            ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake # cmake_build-debug/
#        DESTINATION
#            ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}) # share/

# Create export set to enable import in other CMake Projects
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-targets # create export set
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib/
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # lib/
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # include/
)
# Install Export Set
install(EXPORT ${PROJECT_NAME}-targets
        NAMESPACE nanojsonc::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME})

# Install Headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}) # include/

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.1 AND NOT WIN32) # sanitizer
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -fno-sanitize-recover=all")
#    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined -fno-sanitize-recover=all)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address,undefined)
endif ()

if (CMAKE_COMPILER_IS_GNUCC AND NOT WIN32) # werror
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wall -ansi")
#    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ansi -pedantic")
endif ()

option(BUILD_TESTS "Build Tests" ON) # Tests
if (BUILD_TESTS)
#    enable_testing()
#    include(CTest)

#    add_subdirectory(test)
#    add_subdirectory(example)
endif ()
