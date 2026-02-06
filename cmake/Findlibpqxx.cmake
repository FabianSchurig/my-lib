# FindLibpqxx.cmake
# Find libpqxx (C++ client library for PostgreSQL)
#
# This module defines:
#  LIBPQXX_FOUND - system has libpqxx
#  LIBPQXX_INCLUDE_DIRS - the libpqxx include directory
#  LIBPQXX_LIBRARIES - Link these to use libpqxx
#  LIBPQXX_VERSION - Version of libpqxx

find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_LIBPQXX QUIET libpqxx)
endif()

find_path(LIBPQXX_INCLUDE_DIR
    NAMES pqxx/pqxx
    HINTS ${PC_LIBPQXX_INCLUDEDIR} ${PC_LIBPQXX_INCLUDE_DIRS}
    PATH_SUFFIXES pqxx
)

find_library(LIBPQXX_LIBRARY
    NAMES pqxx libpqxx
    HINTS ${PC_LIBPQXX_LIBDIR} ${PC_LIBPQXX_LIBRARY_DIRS}
)

if(PC_LIBPQXX_VERSION)
    set(LIBPQXX_VERSION ${PC_LIBPQXX_VERSION})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libpqxx
    REQUIRED_VARS LIBPQXX_LIBRARY LIBPQXX_INCLUDE_DIR
    VERSION_VAR LIBPQXX_VERSION
)

if(libpqxx_FOUND AND NOT TARGET pqxx)
    add_library(pqxx UNKNOWN IMPORTED)
    set_target_properties(pqxx PROPERTIES
        IMPORTED_LOCATION "${LIBPQXX_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${LIBPQXX_INCLUDE_DIR}"
    )
    
    # libpqxx requires libpq
    find_package(PostgreSQL REQUIRED)
    set_target_properties(pqxx PROPERTIES
        INTERFACE_LINK_LIBRARIES "PostgreSQL::PostgreSQL"
    )
endif()

mark_as_advanced(LIBPQXX_INCLUDE_DIR LIBPQXX_LIBRARY)

set(LIBPQXX_INCLUDE_DIRS ${LIBPQXX_INCLUDE_DIR})
set(LIBPQXX_LIBRARIES ${LIBPQXX_LIBRARY})
