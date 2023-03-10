# Create the library
add_library(dachshund_render_engine STATIC 
	"src/RenderingManager.cxx"
	"src/DirectTextureContainer.cxx"
	"src/RenderScene.cxx"
		)

# Create alias for the library for better looking
add_library(SDL2::RenderEngine ALIAS dachshund_render_engine)

# Set target include directories
target_include_directories(dachshund_render_engine
	PUBLIC
		.
		texture_components
		engine_interfaces
)

# Set target include directories
target_include_directories(dachshund_direct_engine
		PUBLIC
		include
)

# Setup dependency include directories
target_include_directories(dachshund_render_engine PUBLIC ${DEP_INSTALL_DIR}/include)

# Link dependencies
target_link_libraries(dachshund_render_engine PUBLIC -L${DEP_INSTALL_DIR}/lib)

# Link the library against the other libraries
if(UNIX)
	target_include_directories(dachshund_render_engine PUBLIC include ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
	target_link_libraries(dachshund_render_engine      PUBLIC ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES})
else()
	target_link_libraries(dachshund_render_engine SDL2::SDL2main SDL2::SDL2)
	target_link_libraries(dachshund_render_engine SDL2_image::SDL2_image)
	target_link_libraries(dachshund_render_engine SDL2_mixer::SDL2_mixer)
	target_link_libraries(dachshund_render_engine SDL2_ttf::SDL2_ttf)
endif(UNIX)