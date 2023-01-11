add_library(dachshund_render_engine
	"RenderingManager.cxx"
	"RenderingManager.h"
)

target_include_directories(dachshund_render_engine
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dachshund_render_engine SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(dachshund_render_engine SDL2_image::SDL2_image)
target_link_libraries(dachshund_render_engine SDL2_mixer::SDL2_mixer)
target_link_libraries(dachshund_render_engine SDL2_ttf::SDL2_ttf)

