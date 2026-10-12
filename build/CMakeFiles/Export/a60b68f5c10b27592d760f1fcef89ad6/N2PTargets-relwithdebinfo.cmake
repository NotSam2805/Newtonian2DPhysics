#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "N2P::N2P" for configuration "RelWithDebInfo"
set_property(TARGET N2P::N2P APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(N2P::N2P PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELWITHDEBINFO "CXX"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/N2P.lib"
  )

list(APPEND _cmake_import_check_targets N2P::N2P )
list(APPEND _cmake_import_check_files_for_N2P::N2P "${_IMPORT_PREFIX}/lib/N2P.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
