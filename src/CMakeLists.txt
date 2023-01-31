# Add nested folders
add_subdirectory(direct_engine)
#add_subdirectory(test)

# Add source to this project's executable.
add_executable (dachshund 
	"EntryPoint.cxx"	 
)

# We use the library for each subfolder
target_link_libraries(dachshund
	dachshund_direct_engine
	dachshund_render_engine
	dachshund_interface_engine
)

# Add the SDL2 include paths
if(WIN32)
target_link_libraries(dachshund mingw32)
endif(WIN32)

target_link_libraries(dachshund SDL2::Engine)
target_link_libraries(dachshund SDL2::RenderEngine)

set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -fno-pie")
set (CMAKE_C_FLAGS_RELWITHDEBINFO   "${CMAKE_C_FLAGS_RELWITHDEBINFO} -fno-pie")


if(UNIX)
	target_include_directories(dachshund PUBLIC include ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
	target_link_libraries(dachshund ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2MIXER_LIBRARIES})
else()
	target_link_libraries(dachshund SDL2::SDL2main SDL2::SDL2)
	target_link_libraries(dachshund SDL2_image::SDL2_image)
	target_link_libraries(dachshund SDL2_mixer::SDL2_mixer)
	target_link_libraries(dachshund SDL2_ttf::SDL2_ttf)
endif(UNIX)

# Copy yhe executable to the release folder of the project
set_target_properties(dachshund PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/bin/
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/
)