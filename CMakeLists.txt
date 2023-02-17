# CMakeList.txt : CMake project for dachshund, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.8)

# Set the project name and the version
project ("dachshund" VERSION 0.1)
include(cmake/StandardProjectSettings.cmake)

# Set the c++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup google test environment
#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
  if (ENABLE_BUILD_WITH_TIME_TRACE)
    add_compile_definitions(project_options INTERFACE -ftime-trace)
  endif()
endif()

# Include the formula for the RapidJSON library
include(vendor/Dependencies.cmake)
include(vendor/RapidJSON.cmake)
include(vendor/SpeedLogging.cmake)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# Enable cache system
include(cmake/Cache.cmake)

# Standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# Enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
  # This sets a global PCH parameter, each project will build its own PCH, which
  # is a good idea if any #define's change
  #
  # consider breaking this out per project as necessary
  target_precompile_headers(project_options INTERFACE <vector> <string> <map> <utility> <unordered_map>)
endif()

# Allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

# Locate the SDL2 libraries
if(UNIX)
  INCLUDE(FindPkgConfig)
  PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
  PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
  PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
  PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
else()
  find_package(SDL2       REQUIRED)
  find_package(SDL2_ttf   REQUIRED)
  find_package(SDL2_image REQUIRED)
  find_package(SDL2_mixer REQUIRED)
endif(UNIX)

# Set output executable path to the proper folder
set(EXECUTABLE_OUTPUT_PATH "bin")

# Add nested directories
add_subdirectory(src)

# Make the progran properly run in the visual studio
set_target_properties(
    dachshund PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET dachshund PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
