# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\yinyangshi_global_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\yinyangshi_global_autogen.dir\\ParseCache.txt"
  "yinyangshi_global_autogen"
  )
endif()
