#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <EngineInterface.h>

int main(int argc, char *argv[])
{
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_DEBUG);

	SDL::EngineInterface game_engine;
	game_engine.start();
	game_engine.stop();

	return 0;
}
