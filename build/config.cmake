#@PACKAGE_INIT@
#include(${CMAKE_CURRENT_LIST_DIR}/nanojsonc-targets.cmake)
#check_required_components(nanojsonc)

include(CMakeFindDependencyMacro)

# Declare the dependency on nanojsonc
find_dependency(nanojsonc REQUIRED)

# Create imported target
add_library(nanojsonc::nanojsonc INTERFACE IMPORTED)

# Set target properties
set_target_properties(nanojsonc::nanojsonc PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CURRENT_PACKAGES_DIR}/include"
        INTERFACE_LINK_LIBRARIES "${CURRENT_PACKAGES_DIR}/lib/libnanojsonc.a"
)
