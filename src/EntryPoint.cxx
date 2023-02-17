#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <DirectTextureContainer.h>
#include <DirectTextureFactory.h>
#include <RenderingManager.h>
#include <EngineInterface.h>
#include <RenderScene.h>

#include "spdlog/spdlog.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

    std::shared_ptr<SDL::RenderingManager>      render_manager  (new SDL::RenderingManager());
    std::shared_ptr<SDL::EngineInterface>       game_engine     (new SDL::EngineInterface(render_manager));

	SDL::RenderScene menu_scene(game_engine->get_renderer_handle());
    SDL::RenderScene game_scene(game_engine->get_renderer_handle());
    SDL::RenderScene test_scene(game_engine->get_renderer_handle());

	menu_scene.push_to_render_group(1, std::move(SDL::DirectTextureContainer(
            "data/assets/test.jpg",
            {0, 0, 100, 100},
            {0, 0, 100, 100}
    )));

    menu_scene.add_supported_font("roboto-light",
        SDL::DirectFontContainer(
           "data/fonts/GoogleRoboto/Roboto-Light.ttf", 24,
            SDL::DirectFontDisplayVariant :: Normal,
            SDL::DirectFontRenderKind     :: Blended
        )
    );

    menu_scene.push_text_to_render_group(1,
        "roboto-light",
        "Test test",
        {255, 100, 255, 255},
        {0,   0,   0,   255},
        {200, 200}
    );

    menu_scene.enable_render_group (1);

    render_manager->bind_render_scene("menu_scene", menu_scene);
    render_manager->set_active_render_scene("menu_scene");

	game_engine->start();
	game_engine->stop();

    menu_scene.disable_render_group (1);

	return 0;
}
