# Create the library
add_library(dachshund_render_engine STATIC 
	"RenderingManager.h"	
	"RenderingManager.cxx"
	"RenderingEngine.h"
	"DirectTextureFactory.h"
	"RenderSceneFactory.cxx"
	"DirectTextireContainer.h"
	"RenderScene.h" 
	"RenderScene.cxx"
	"RenderObject.h"
        RenderingExceptions.h DirectTextureFactory.cxx)

# Create alias for the library for better looking
add_library(SDL2::RenderEngine ALIAS dachshund_render_engine)

# Set target include directories
target_include_directories(dachshund_render_engine
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Link the library against the other libraries
target_link_libraries(dachshund_render_engine
		SDL2::SDL2 
		SDL2::SDL2main
		SDL2_ttf::SDL2_ttf
		SDL2_image::SDL2_image
		SDL2_mixer::SDL2_mixer
)