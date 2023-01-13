#pragma once

#include <utility>
#include <cstdint>

namespace SDL::priv
{
	class EngineRegistry
	{
	public:
		std::pair<uint32_t, uint32_t> m_current_window_dimensions;
		std::pair<uint32_t, uint32_t> m_current_window_position;
	};
}
