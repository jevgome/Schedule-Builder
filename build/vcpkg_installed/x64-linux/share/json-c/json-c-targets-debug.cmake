#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "json-c::json-c" for configuration "Debug"
set_property(TARGET json-c::json-c APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(json-c::json-c PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/debug/lib/libjson-c.a"
  )

list(APPEND _cmake_import_check_targets json-c::json-c )
list(APPEND _cmake_import_check_files_for_json-c::json-c "${_IMPORT_PREFIX}/debug/lib/libjson-c.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
