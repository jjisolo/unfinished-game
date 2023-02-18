#include "../include/backend/EngineInterface.h"

static constexpr const uint32_t mixer_init_flags  = MIX_INIT_MP3;
static constexpr const uint32_t image_init_flags  = IMG_INIT_PNG   | IMG_INIT_JPG;
static constexpr const uint32_t direct_init_flags = SDL_INIT_VIDEO | SDL_INIT_AUDIO;

static constexpr const char* default_window_title = "Dachshund adventures!";

SDL_Renderer* SDL::EngineInterface::get_renderer_handle()
{
	return m_renderer_handle.get();
}

[[maybe_unused]] SDL::DirectWindowPosition SDL::EngineInterface::get_window_position()
{
	SDL::DirectWindowPosition window_position;
	SDL_GetWindowPosition(m_window_handle.get(), &window_position.first, &window_position.second);
	return window_position;
}

[[maybe_unused]] SDL::DirectWindowPosition SDL::EngineInterface::get_window_dimensions()
{
	SDL::DirectWindowPosition window_dimensions;
	SDL_GetWindowSize(m_window_handle.get(), &window_dimensions.first, &window_dimensions.second);
	return window_dimensions;
}

void SDL::EngineInterface::builtin_on_user_create()
{
    spdlog::info("Initializing SDL2 subsystems...");

	if (SDL_Init(direct_init_flags) != 0)
	{
        spdlog::critical("SDL subsystems initialization failed");
        spdlog::critical("{}", SDL_GetError());

		m_application_should_close = true;
	}

	if (TTF_Init() != 0)
	{
        spdlog::critical("SDL_ttf subsystems initialization failed");
        spdlog::critical("{}", TTF_GetError());

		m_application_should_close = true;
	}

	if (!(IMG_Init(image_init_flags) & image_init_flags))
	{
        spdlog::critical("SDL_image subsystems initialization failed");
        spdlog::critical("{}", IMG_GetError());

		m_application_should_close = true;
	}

	if (!(Mix_Init(mixer_init_flags) & mixer_init_flags))
	{
        spdlog::critical("SDL_mixer subsystems initialization failed");
        spdlog::critical("{}", Mix_GetError());

		m_application_should_close = true;
	}

    spdlog::info("SDL2 subsystems initialized succesfully.");
    spdlog::info("Initializing basic SDL2 structures.");
    spdlog::info("Loading the window properties data.");

	if (!m_window_startup_details.load())
	{
        spdlog::debug("As the read operation failed, the default launch info is going to be used");

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

		m_registry.m_current_window_dimensions = std::make_pair(
			SDL::priv::default_window_position.first,
			SDL::priv::default_window_position.second
		);
	}
	else
	{
        spdlog::debug("Read operation succeeded, using the previously saved launch info");

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
			static_cast<int>(m_window_startup_details.m_datum.m_last_known_rendering_device),
			                 m_window_startup_details.m_datum.m_last_known_renderer_flags
		));

		m_registry.m_current_window_dimensions = std::make_pair(
			m_window_startup_details.m_datum.m_last_known_dimensions.first, 
			m_window_startup_details.m_datum.m_last_known_dimensions.second
		);
	}

	if (m_window_handle.get()   == nullptr)
	{
        spdlog::critical("SDL_Window structure initialization failed!");
        spdlog::critical("{}", SDL_GetError());
	}
	
	if (m_renderer_handle.get() == nullptr)
	{
        spdlog::critical("SDL_Renderer structure initialization failed!");
        spdlog::critical("{}", SDL_GetError());
	}

    spdlog::info("SDL2 structures initialized succesfully");
	m_application_should_close = false;
}

void SDL::EngineInterface::builtin_on_user_update()
{
    // A union that contains structures for the different event types.
    SDL_Event processed_event;

    // Pump the event loop, gathering events from the input devices
    SDL_PumpEvents();

    // Update the state of the mouse and keyboard
    m_keyboard_state -> update(processed_event);
    m_mouse_state    -> update(processed_event);

    // All SDL2 system events
    for (;SDL_PollEvent(&processed_event);)
	{
		switch (processed_event.type)
		{
            // SDL_QUIT: User hit the quit button on the application window
			case SDL_QUIT:
				m_application_should_close = true;
				break;
		}
	}
}

void SDL::EngineInterface::start()
{
	while (m_application_should_close != true)
	{
        m_binded_render_manager->render();

		builtin_on_user_update();
		on_user_update();
	}
}

void SDL::EngineInterface::stop()
{
    if(SDL_WasInit(SDL_INIT_VIDEO))
    {
        spdlog::info("Saving the window properties data");

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

        // TODO: rendering device and rendering flags change implementation
        m_window_startup_details.m_datum.m_last_known_rendering_device = SDL::priv::default_rendering_device;
        m_window_startup_details.m_datum.m_last_known_renderer_flags   = SDL::priv::default_renderer_flags;
        m_window_startup_details.m_datum.m_last_known_window_flags     = SDL::priv::default_window_flags;
        m_window_startup_details.save();

        spdlog::info("Deinitializing the SDL2 subsystems");

        if(SDL_WasInit(SDL_INIT_VIDEO)) SDL_Quit();
        if(TTF_WasInit())               TTF_Quit();
                                        IMG_Quit();
                                        Mix_Quit();

    }
    else
    {
        spdlog::error("Attempted to stop unitialized engine");
        throw SDL::DirectSystemException();
    }
}

[[maybe_unused]] void SDL::EngineInterface::resize_window(const std::uint32_t width, const std::uint32_t height, const BinaryState16 logic_resize)
{
	spdlog::debug(
		"\t--- Window resized from ({}, {}) to ({}, {})",
		m_registry.m_current_window_dimensions.first,
		m_registry.m_current_window_dimensions.second,
		width, height
	);

    // Update the window dimensions in the internal window registry
	m_registry.m_current_window_dimensions = std::make_pair(width, height);

    // If it is not a logic resize, update the actual window
	if (!logic_resize) SDL_SetWindowSize(m_window_handle.get(), static_cast<int>(width), static_cast<int>(height));
}

bool SDL::EngineInterface::on_user_update()
{

	return true;
}

bool SDL::EngineInterface::on_user_create()
{

	return true;
}
