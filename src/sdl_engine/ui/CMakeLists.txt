add_library(dachshund_ui_engine
	"UserInterfaceManager.cxx"
	"UserInterfaceManager.h"
	"UserInterfaceObject.cxx"
	"UserInterfaceObject.h"
	"UserInterface.h"
)

target_include_directories(dachshund_ui_engine
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(dachshund_ui_engine SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(dachshund_ui_engine SDL2_image::SDL2_image)
target_link_libraries(dachshund_ui_engine SDL2_mixer::SDL2_mixer)
target_link_libraries(dachshund_ui_engine SDL2_ttf::SDL2_ttf)

