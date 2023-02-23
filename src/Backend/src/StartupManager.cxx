#include <Backend/StartupManager.h>

bool SDL::priv::StartupManager::save(SDL::priv::WindowStartupDetails& details)
{
	SDL_RWops* startup_details = SDL_RWFromFile(SDL::priv::launch_details_filename, "w");

	if (startup_details == NULL)
	{
        spdlog::error("The open operation has failed");
        spdlog::error("{}", SDL_GetError());
		return false;
	}
	
	if (SDL_RWwrite(startup_details, &details, sizeof(details), 1) < 1)
	{
        spdlog::error("The write operation has failed");
        spdlog::error("{}", SDL_GetError());
		return false;
	}

	if (SDL_RWclose(startup_details) != 0)
	{
        spdlog::error("The close operation has failed");
        spdlog::error("{}", SDL_GetError());
		return false;
	}

	return true;
}

std::optional<SDL::priv::WindowStartupDetails> SDL::priv::StartupManager::load()
{
    SDL_RWops* startup_details = SDL_RWFromFile(SDL::priv::launch_details_filename, "r");

    SDL::priv::WindowStartupDetails details;

	if (startup_details != NULL)
	{
		if (SDL_RWread(startup_details, &details, sizeof(details), 1) == 0)
		{
            spdlog::error("The read operation has failed");
            spdlog::error("{}", SDL_GetError());
			return std::nullopt;
		}

		if (SDL_RWclose(startup_details) != 0)
		{
            spdlog::error("The close operation has failed");
            spdlog::error("{}", SDL_GetError());
            return details;
		}

		return details;
	}

	return std::nullopt;
}
