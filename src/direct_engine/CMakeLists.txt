add_library(dachshund_direct_engine STATIC
	"StartupManager.cxx"
	"StartupManager.h"
	"EngineInterface.cxx"
	"EngineInterface.h"
	"Engine.h"
)

target_include_directories(dachshund_direct_engine
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dachshund_direct_engine SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(dachshund_direct_engine SDL2_image::SDL2_image)
target_link_libraries(dachshund_direct_engine SDL2_mixer::SDL2_mixer)
target_link_libraries(dachshund_direct_engine SDL2_ttf::SDL2_ttf)
