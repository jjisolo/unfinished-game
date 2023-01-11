# Add nested folders
add_subdirectory(sdl_engine)

# Add source to this project's executable.
add_executable (dachshund 
	"EntryPoint.cxx"	 
)

# Link the nested directories
link_directories(sdl_engine)
link_directories(rendering)
link_directories(ui)

# We use the library for each subfolder
target_link_libraries(dachshund dachshund_render_engine)
target_link_libraries(dachshund dachshund_sdl_engine)
target_link_libraries(dachshund dachshund_ui_engine)

# Add the SDL2 include paths
target_link_libraries(dachshund SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(dachshund SDL2_image::SDL2_image)
target_link_libraries(dachshund SDL2_mixer::SDL2_mixer)
target_link_libraries(dachshund SDL2_ttf::SDL2_ttf)
