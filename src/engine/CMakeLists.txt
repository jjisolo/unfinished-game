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
    "MouseState.h"
	"KeyboardState.cxx"
	"KeyboardState.h"
)

# Create alias for the library for better looking
add_library(SDL2::Engine ALIAS dachshund_direct_engine)

# Set target include directories
target_include_directories(dachshund_direct_engine
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Setup dependency include directories
target_include_directories(dachshund_direct_engine PUBLIC ${DEP_INSTALL_DIR}/include)

# Link dependencies
target_link_libraries(dachshund_direct_engine PUBLIC -L${DEP_INSTALL_DIR}/lib)

# Link the library against the other libraries
target_link_libraries(dachshund_direct_engine PUBLIC dachshund_render_engine)

if(UNIX)
	target_include_directories(dachshund_direct_engine PUBLIC include ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
	target_link_libraries(dachshund_direct_engine PUBLIC ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2MIXER_LIBRARIES})
else()
	target_link_libraries(dachshund_direct_engine SDL2::SDL2main SDL2::SDL2)
	target_link_libraries(dachshund_direct_engine SDL2_image::SDL2_image)
	target_link_libraries(dachshund_direct_engine SDL2_mixer::SDL2_mixer)
	target_link_libraries(dachshund_direct_engine SDL2_ttf::SDL2_ttf)
endif(UNIX)
