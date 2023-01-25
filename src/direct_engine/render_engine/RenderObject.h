#pragma once

#include <RenderingEngine.h>
#include <DirectTextureContainer.h>

namespace SDL
{
    using SharedDirectSurface  = std::shared_ptr<SDL_Surface>;

	using RenderObjectTitle     = std::string;
	using RenderObjectSize      = std::pair<std::uint16_t, std::uint16_t>;
	using RenderObjectPosition  = std::pair<std::uint16_t, std::uint16_t>;

	using RenderObjectBackend = std::variant<
		SDL::SharedDirectTexture,
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


	struct RenderObject
	{
		RenderObjectKind      kind;       // Enabled(textures are loaded) or Disabled
		RenderObjectType      type;       // DirectTexture(shared), ...
		RenderObjectBackend   backend;    // Actual backend that type defined in field `type`
        RenderObjectTitle     title;      // Title of the render object(debug)
		RenderObjectSize      size;       // Size of the texture
		RenderObjectPosition  position;   // Its position on the screen
	};
}