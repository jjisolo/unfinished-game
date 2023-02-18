#include "../include/backend/StartupManager.h"


bool SDL::priv::WindowStartupDetails::save()
{
    spdlog::debug(" Opening file {} for writing", SDL::priv::launch_details_filename);
	SDL_RWops* startup_details = SDL_RWFromFile(SDL::priv::launch_details_filename, "w");

	if (startup_details == NULL)
	{
        spdlog::error("The open operation has failed");
        spdlog::error("{}", SDL_GetError());
		return false;
	}
	
	if (SDL_RWwrite(startup_details, &m_datum, sizeof(m_datum), 1) < 1)
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

bool SDL::priv::WindowStartupDetails::load()
{
    spdlog::debug("Opening file {} for writing", SDL::priv::launch_details_filename);
	SDL_RWops* startup_details = SDL_RWFromFile(SDL::priv::launch_details_filename, "r");

	if (startup_details != NULL)
	{
		if (SDL_RWread(startup_details, &m_datum, sizeof(m_datum), 1) == 0)
		{
            spdlog::error("The read operation has failed");
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

	return false;
}
