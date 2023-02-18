#pragma once

// SDl2 library headers (https://wiki.libsdl.org/SDL2/FrontPage)
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

// Spdlog logging library headers (https://github.com/gabime/spdlog)
#include <spdlog/spdlog.h>

// Standard library includes
#include <cstdio>
#include <string>
#include <utility>
#include <fstream>
#include <optional>

/*! This is nested private namespace, that is never accessed by the user */
namespace SDL::priv
{
    /*! Default launch info filename */
	static constexpr const char* launch_details_filename = "LaunchInfo.bin";

    #pragma pack(push, 1)
    struct WindowStartupDetails
    {
        std::pair<int, int> m_last_known_position;
        std::pair<int, int> m_last_known_dimensions;

        uint32_t m_last_known_window_flags;
        uint32_t m_last_known_renderer_flags;
        uint32_t m_last_known_rendering_device;
    };
    #pragma pack(pop)

    /*! This class represents a packed data structure that containing
     *  relevant information about the application window(size, position,
     *  etc.).
     *
     *  \note This class is copyable and movable
     */
	class StartupManager
	{
	public:
        /*!
         * Save the window properties data
         *
         * \param details A data structure WindowStartupDetails
         * that is going to be saved
         * \returns bool False if the file could not be saved
         */
		static bool save(WindowStartupDetails& details);

        /*!
         * Load the window launch data
         *
         * \param details A data structure WindowStartupDetails
         * that is going to be saved
         * \returns bool False if the file could not be saved
         */
		static std::optional<WindowStartupDetails> load();
	};
}