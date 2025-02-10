# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\CommonLib_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CommonLib_autogen.dir\\ParseCache.txt"
  "CommonLib_autogen"
  )
endif()
