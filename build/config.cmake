@PACKAGE_INIT@

include(${CMAKE_CURRENT_LIST_DIR}/nanojsonc-targets.cmake)

check_required_components(nanojsonc)

#include(GNUInstallDirs)
#include(CMakeFindDependencyMacro)
#
## Declare the dependency on nanojsonc
#find_dependency(nanojsonc REQUIRED)
#
## Create imported target
#add_library(nanojsonc::nanojsonc INTERFACE IMPORTED)
#
## Set target properties
#set_target_properties(nanojsonc::nanojsonc PROPERTIES
#        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_INSTALL_INCLUDEDIR}/nanojsonc"
#        INTERFACE_LINK_LIBRARIES "${CMAKE_INSTALL_LIBDIR}/libnanojsonc.a"
#)
