#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <spdlog/spdlog.h>

#include <cstdio>
#include <string>
#include <utility>
#include <fstream>

namespace SDL::priv
{
	static constexpr const char* launch_details_filename = "LaunchInfo.bin";

	class WindowStartupDetails
	{
	public:
		bool save();
		bool load();

	public:
		#pragma pack(push, 1) 
		struct
		{
			std::pair<int, int> m_last_known_position;
			std::pair<int, int> m_last_known_dimensions;

			uint32_t m_last_known_window_flags;
			uint32_t m_last_known_renderer_flags;
			uint32_t m_last_known_rendering_device;
		} m_datum;
		#pragma pack(pop)
	};
}