# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.29
cmake_policy(SET CMP0009 NEW)

# ASSETS_FILES at CMakeLists.txt:38 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/rupert/projects/SAR-SavingAmazonRainforest/assets/*")
set(OLD_GLOB
  "/home/rupert/projects/SAR-SavingAmazonRainforest/assets/night_wolf_background.jpg"
  "/home/rupert/projects/SAR-SavingAmazonRainforest/assets/nyarch.png"
  "/home/rupert/projects/SAR-SavingAmazonRainforest/assets/uwuntu.png"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/rupert/projects/SAR-SavingAmazonRainforest/build/CMakeFiles/cmake.verify_globs")
endif()

# SRC_FILES at CMakeLists.txt:29 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/rupert/projects/SAR-SavingAmazonRainforest/dependencies/src/*.c")
set(OLD_GLOB
  "/home/rupert/projects/SAR-SavingAmazonRainforest/dependencies/src/gl.c"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/rupert/projects/SAR-SavingAmazonRainforest/build/CMakeFiles/cmake.verify_globs")
endif()

# SRC_FILES at CMakeLists.txt:29 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/rupert/projects/SAR-SavingAmazonRainforest/dependencies/src/*.cpp")
set(OLD_GLOB
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/rupert/projects/SAR-SavingAmazonRainforest/build/CMakeFiles/cmake.verify_globs")
endif()

# SRC_FILES at CMakeLists.txt:29 (file)
# SRC_FILES at CMakeLists.txt:29 (file)
file(GLOB_RECURSE NEW_GLOB LIST_DIRECTORIES false "/home/rupert/projects/SAR-SavingAmazonRainforest/src/*.cpp")
set(OLD_GLOB
  "/home/rupert/projects/SAR-SavingAmazonRainforest/src/main.cpp"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/rupert/projects/SAR-SavingAmazonRainforest/build/CMakeFiles/cmake.verify_globs")
endif()
