# Add nested folders
add_subdirectory(direct_engine)
add_subdirectory(interface_engine)

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
target_link_libraries(dachshund SDL2::Engine)
target_link_libraries(dachshund SDL2::RenderEngine)
target_link_libraries(dachshund SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(dachshund SDL2_image::SDL2_image)
target_link_libraries(dachshund SDL2_mixer::SDL2_mixer)
target_link_libraries(dachshund SDL2_ttf::SDL2_ttf)

# Copy yhe executable to the release folder of the project
set_target_properties(dachshund PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/bin/
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/
)