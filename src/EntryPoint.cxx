#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <EngineInterface.h>
#include <DirectTextureContainer.h>
#include <DirectTextureFactory.h>
#include <RenderingManager.h>
#include <RenderScene.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    std::shared_ptr<SDL::RenderingManager>      render_manager  (new SDL::RenderingManager());
    std::shared_ptr<SDL::DirectTextureFactory>  texture_factory (new SDL::DirectTextureFactory("data/assets/undefined.png"));
    std::shared_ptr<SDL::EngineInterface>       game_engine     (new SDL::EngineInterface(render_manager));

	SDL::RenderScene menu_scene(game_engine->get_renderer_handle(), texture_factory);
    SDL::RenderScene game_scene(game_engine->get_renderer_handle(), texture_factory);
    SDL::RenderScene test_scene(game_engine->get_renderer_handle(), texture_factory);

	menu_scene.push_to_render_group(1, std::move(SDL::DirectTextureContainer(
            SDL::DirectTextureContainerLoadVariant::LoadImage,
            "data/assets/test.jpg",
            {0, 0, 100, 100},
            {0, 0, 100, 100}
    )));

    menu_scene.push_to_render_group(1, std::move(SDL::DirectTextureContainer(
            SDL::DirectTextureContainerLoadVariant::LoadImage,
            "data/assets/failed.jpg",
            {0, 0, 100, 100},
            {100, 100, 100, 100}
    )));


    menu_scene.enable_render_group (1);

    render_manager->bind_render_scene("menu_scene", menu_scene);
    render_manager->set_active_render_scene("menu_scene");

	game_engine->start();
	game_engine->stop();

    menu_scene.disable_render_group (1);

	return 0;
}
