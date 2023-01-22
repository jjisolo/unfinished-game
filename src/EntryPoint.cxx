﻿#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <EngineInterface.h>
#include <DirectTextureFactory.h>
#include <RenderScene.h>

int main(int argc, char *argv[])
{
	using std::operator""s;

	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);
	SDL_LogSetPriority(SDL_LOG_CATEGORY_SYSTEM,		 SDL_LOG_PRIORITY_ERROR);

	std::shared_ptr<SDL::EngineInterface> game_engine(new SDL::EngineInterface());

	SDL::RenderScene  menu_scene;

	SDL::RenderObject background = {
		SDL::RenderObjectKind::Enabled,
		SDL::RenderObjectType::DirectTexture,
		SDL::DirectTextureFactory::create_texture_from_image(game_engine->get_renderer_handle(), "asdsad.png"),
		"menu_background_texture",
		{100, 100}, {10, 10}
	};

	SDL::RenderObject background2 = {
		SDL::RenderObjectKind::Enabled,
		SDL::RenderObjectType::DirectTexture,
		SDL::DirectTextureFactory::create_texture_from_image(game_engine->get_renderer_handle(), "asdsad.png"),
		"menu_background_texture",
		{100, 100}, {10, 10}
	};


	menu_scene.alias_render_group(1, "main_menu"s);
	menu_scene.push_to_render_group("main_menu"s, std::move(background));
	menu_scene.push_to_render_group(1, std::move(background2));


	game_engine->start();
	game_engine->stop();

	return 0;
}