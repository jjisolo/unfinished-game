#include <EngineInterface.h>

static constexpr const uint32_t mixer_init_flags  = MIX_INIT_MP3;
static constexpr const uint32_t image_init_flags  = IMG_INIT_PNG   | IMG_INIT_JPG;
static constexpr const uint32_t direct_init_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;

static constexpr const char* default_window_title = "Dachshund adventures!";

void SDL::EngineInterface::builtin_on_user_create()
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::EngineInterface::builtin_on_user_create [%p]", &SDL::EngineInterface::builtin_on_user_create);
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Initializing SDL2 subsystems.");

	if (SDL_Init(direct_init_flags) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- SDL subsystems initialization failed!");
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", SDL_GetError());

		m_application_should_close = true;
	}

	if (TTF_Init() != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- SDL_ttf subsystems initialization failed!");
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", TTF_GetError());

		m_application_should_close = true;
	}

	if (!(IMG_Init(image_init_flags) & image_init_flags))
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- SDL_image subsystems initialization failed!");
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", IMG_GetError());

		m_application_should_close = true;
	}

	if (!(Mix_Init(mixer_init_flags) & mixer_init_flags))
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- SDL_mixer subsystems initialization failed!");
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", Mix_GetError());

		m_application_should_close = true;
	}

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- SDL2 subsystems initialized succesfully.");
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Initializing basic SDL2 structures.");

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Loading the window properties data.");

	if (m_window_startup_details.load() == false)
	{
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- As the read operation failed, the default launch info is going to be used");

		m_window_handle.reset(SDL_CreateWindow(
			SDL::priv::default_window_title,
			SDL::priv::default_window_position.first,
			SDL::priv::default_window_position.second,
			SDL::priv::default_window_dimensions.first,
			SDL::priv::default_window_dimensions.second,
			SDL::priv::default_window_flags
		));

		m_renderer_handle.reset(SDL_CreateRenderer(
			m_window_handle.get(),
			SDL::priv::default_rendering_device,
			SDL::priv::default_renderer_flags
		));
	}
	else
	{
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Using the previously saved launch info");

		m_window_handle.reset(SDL_CreateWindow(
			SDL::priv::default_window_title,
			m_window_startup_details.m_datum.m_last_known_position.first,
			m_window_startup_details.m_datum.m_last_known_position.second,
			m_window_startup_details.m_datum.m_last_known_dimensions.first,
			m_window_startup_details.m_datum.m_last_known_dimensions.second,
			m_window_startup_details.m_datum.m_last_known_window_flags
		));

		m_renderer_handle.reset(SDL_CreateRenderer(
			m_window_handle.get(),
			m_window_startup_details.m_datum.m_last_known_rendering_device,
			m_window_startup_details.m_datum.m_last_known_renderer_flags
		));
	}

	if (m_window_handle.get()   == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- SDL_Window structure initialization failed!");
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", SDL_GetError());
	}
	
	if (m_renderer_handle.get() == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- SDL_Renderer structure initialization failed!");
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", SDL_GetError());
	}

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- SDL2 structures initialized succesfully");
	m_application_should_close = false;
}

void SDL::EngineInterface::builtin_on_user_update()
{
	for (SDL_Event processed_event; SDL_PollEvent(&processed_event);)
	{
		switch (processed_event.type)
		{
			case SDL_QUIT:
			{
				SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "The application recieved QUIT signal, proceeding to the program exit");
				m_application_should_close = true;
			}
		}
	}
}

void SDL::EngineInterface::start()
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::EngineInterface::start [%p]", &SDL::EngineInterface::start);

	while (m_application_should_close != true)
	{
		builtin_on_user_update();
		on_user_update();
	}
}

void SDL::EngineInterface::stop()
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::EngineInterface::stop [%p]", &SDL::EngineInterface::stop);
	
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Saving the window properties data");
	SDL_GetWindowPosition(
		m_window_handle.get(),
		&m_window_startup_details.m_datum.m_last_known_position.first,
		&m_window_startup_details.m_datum.m_last_known_position.second
	);

	SDL_GetWindowSize(
		m_window_handle.get(),
		&m_window_startup_details.m_datum.m_last_known_dimensions.first,
		&m_window_startup_details.m_datum.m_last_known_dimensions.second
	);

	m_window_startup_details.m_datum.m_last_known_rendering_device = SDL::priv::default_rendering_device;
	m_window_startup_details.m_datum.m_last_known_renderer_flags   = SDL::priv::default_renderer_flags;
	m_window_startup_details.m_datum.m_last_known_window_flags     = SDL::priv::default_window_flags;
	m_window_startup_details.save();

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Deinitializing the SDL2 subsystems");
	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
	Mix_Quit();
}

bool SDL::EngineInterface::on_user_update()
{

	return true;
}

bool SDL::EngineInterface::on_user_create()
{

	return true;
}