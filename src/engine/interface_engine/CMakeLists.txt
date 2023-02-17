add_library(dachshund_interface_engine STATIC
	"UserInterfaceManager.cxx"
	"UserInterfaceManager.h"
	"UserInterfaceObject.cxx"
	"UserInterfaceObject.h"
	"UserInterface.h"
)

target_include_directories(dachshund_interface_engine
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Link the library against the other libraries
if(UNIX)
	target_include_directories(dachshund_interface_engine PUBLIC include ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
	target_link_libraries(dachshund_interface_engine PUBLIC ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2MIXER_LIBRARIES})
else()
	target_link_libraries(dachshund_interface_engine SDL2::SDL2main SDL2::SDL2)
	target_link_libraries(dachshund_interface_engine SDL2_image::SDL2_image)
	target_link_libraries(dachshund_interface_engine SDL2_mixer::SDL2_mixer)
	target_link_libraries(dachshund_interface_engine SDL2_ttf::SDL2_ttf)
endif(UNIX)

