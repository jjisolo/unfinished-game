add_library(dachshund_sdl_engine
	"StartupManager.cxx"
	"StartupManager.h"
	"EngineInterface.cxx"
	"EngineInterface.h"
 "rendering/RenderingManager.h" "rendering/RenderingManager.cxx")

target_include_directories(dachshund_sdl_engine
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dachshund_sdl_engine SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(dachshund_sdl_engine SDL2_image::SDL2_image)
target_link_libraries(dachshund_sdl_engine SDL2_mixer::SDL2_mixer)
target_link_libraries(dachshund_sdl_engine SDL2_ttf::SDL2_ttf)

add_subdirectory(rendering)
add_subdirectory(ui)