#pragma once

#include <RenderingEngine.h>

namespace SDL
{
	class DirectTextureFactory
	{
	public:
		/*!
		* \brief Load the image from given relative path
		*
		* \param renderer_handle A handle to an SDL2 renderer
		* \param image_path      A path to an image
		*/
		static SmartTextureHandle&& create_texture_from_image(SDL_Renderer* renderer_handle, const std::string& image_path)
		{
			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::DirectTextureFactory::load_image [%p]", &SDL::DirectTextureFactory::create_texture_from_image);

			SDL_Surface* image_surface;
			SDL_Texture* image_texture;

			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Loading image %s", image_path.c_str());
			if ((image_surface = IMG_Load(image_path.c_str())) == NULL)
			{
				SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- Image loading failed");
				SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", IMG_GetError());
				return std::move(SmartTextureHandle(nullptr, SDL_DestroyTexture));
			}

			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Serializing image to the texture");
			if ((image_texture = SDL_CreateTextureFromSurface(renderer_handle, image_surface)) == NULL)
			{
				SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- Image convertation to texture failed");
				SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", IMG_GetError());
				return std::move(SmartTextureHandle(nullptr, SDL_DestroyTexture));
			}
			
			SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Cleaning up the image surface");
			SDL_FreeSurface(image_surface);

			return std::move(SmartTextureHandle(image_texture, SDL_DestroyTexture));
		}
	};
}