# Add the nested directories
add_subdirectory(render_engine)
add_subdirectory(interface_engine)

# Create the library
add_library(dachshund_direct_engine STATIC
	"Engine.h"
	"StartupManager.cxx"
	"StartupManager.h"
	"EngineInterface.cxx"
	"EngineInterface.h"
	"EngineRegistry.h"
)

# Create alias for the library for better looking
add_library(SDL2::Engine ALIAS dachshund_direct_engine)

# Set target include directories
target_include_directories(dachshund_direct_engine
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Link the library against the other libraries
target_link_libraries(dachshund_direct_engine
		dachshund_render_engine
		SDL2::SDL2 
		SDL2::SDL2main
		SDL2_ttf::SDL2_ttf
		SDL2_image::SDL2_image
		SDL2_mixer::SDL2_mixer
)
