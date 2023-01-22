#pragma once

#include <RenderingEngine.h>

namespace SDL
{
	using RenderObjectTitle    = std::string;
	using RenderObjectSize     = std::pair<std::uint16_t, std::uint16_t>;
	using RenderObjectPosition = std::pair<std::uint16_t, std::uint16_t>;
	using RenderObjectState    = bool;

	using RenderObjectBackend = std::variant<
		SDL::SmartTextureHandle,
		SDL_Texture*
	>;

	enum class RenderObjectKind: std::uint8_t
	{
		Enabled,
		Disabled,
		//...
	};

	enum class RenderObjectType : std::uint8_t
	{
		DirectTexture,
		//...
	};
	
	struct DirectTextureWrapper
	{
		SDL_Texture*      texture;
		DirectTextureKind kind;
	};

	struct RenderObject
	{
		RenderObjectKind     kind;
		RenderObjectType     type;
		RenderObjectBackend  backend;
		RenderObjectTitle    title;
		RenderObjectSize     size;
		RenderObjectPosition position;
	};
}