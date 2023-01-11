#include <StartupManager.h>


bool SDL::priv::WindowStartupDetails::save()
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::WindowStartupDetails::load [%p]", &SDL::priv::WindowStartupDetails::load);

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Opening %s file for writing", SDL::priv::launch_details_filename);
	SDL_RWops* startup_details = SDL_RWFromFile(SDL::priv::launch_details_filename, "w");

	if (startup_details == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The open operation to file %s has failed", SDL::priv::launch_details_filename);
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", SDL_GetError());
		return false;
	}
	
	if (SDL_RWwrite(startup_details, &m_datum, sizeof(m_datum), 1) < 1)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The write operation to file %s has failed", SDL::priv::launch_details_filename);
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", SDL_GetError());
		return false;
	}

	if (SDL_RWclose(startup_details) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The close operation to file %s has failed", SDL::priv::launch_details_filename);
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", SDL_GetError());
		return false;
	}

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- The LaunchInfo has been saved succesfully");
	return true;
}

bool SDL::priv::WindowStartupDetails::load()
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::WindowStartupDetails::save [%p]", &SDL::priv::WindowStartupDetails::save);

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Opening %s file for reading", SDL::priv::launch_details_filename);
	SDL_RWops* startup_details = SDL_RWFromFile(SDL::priv::launch_details_filename, "r");

	if (startup_details != NULL)
	{
		if (SDL_RWread(startup_details, &m_datum, sizeof(m_datum), 1) == 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The read operation to file %s has failed", SDL::priv::launch_details_filename);
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", SDL_GetError());
			return false;
		}

		if (SDL_RWclose(startup_details) != 0)
		{
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The close operation to file %s has failed", SDL::priv::launch_details_filename);
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", SDL_GetError());
			return false;
		}

		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- The LaunchInfo has been loaded succesfully");
		return true;
	}

	return false;
}
