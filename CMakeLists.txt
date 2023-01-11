# CMakeList.txt : CMake project for dachshund, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Set the project name and the versiom
project ("dachshund" VERSION 0.1)

# Set the c++ standart
set(CMAKE_CXX_STANDARD 20)

# Locate the SDL2 libraries
find_package(SDL2       REQUIRED)
find_package(SDL2_ttf   REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

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
