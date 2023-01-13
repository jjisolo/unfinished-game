#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <string>
#include <memory>

namespace SDL::Render
{
	using SmartTextureHandle  = std::unique_ptr<SDL_Texture,  decltype(&SDL_DestroyTexture)>;
	using SmartRendererHandle = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;

	class DirectTextureFactory
	{
	public:
		DirectTextureFactory(DirectTextureFactory&& texture_wrapper) noexcept:
			m_texture_handle(std::move(texture_wrapper.m_texture_handle)) {}

		DirectTextureFactory():
			m_texture_handle(nullptr, SDL_DestroyTexture) {}

		/*!
		* \brief Get the raw texture pointer
		*/
		SDL_Texture* get();

		/*!
		* \brief Blit another wrapped texture to this texture
		*
		* \param renderer_handle A handle to an SDL2 renderer
		* \param blitted_texture A texture to be blitted onto this texture
		* \param source_rect* Source rectangle
		* \param source_rect* Destination rectangle rectangle
		*/
		void blit_texture(SDL_Renderer* renderer_handle, SDL::Render::DirectTextureFactory& blitted_texture, SDL_Rect* source_rect, SDL_Rect* destination_rect);
		
		/*!
		* \brief Load the image from given relative path
		*
		* \param renderer_handle A handle to an SDL2 renderer
		* \param image_path A path to an image
		*/
		void load_image(SDL_Renderer* renderer_handle, const std::string& image_path);
		
		/*!
		* \brief Render the texture to the screen
		*
		* \param renderer_handle A handle to an SDL2 renderer
		* \param image_path A path to an image
		*/
		void render(SDL_Renderer* renderer_handle, SDL_Rect* source_rect, SDL_Rect* destination_rect);

	private:
		SmartTextureHandle m_texture_handle;
	};
}