#include <DirectTextureFactory.h>

SDL_Texture* SDL::Render::DirectTextureFactory::get()
{
	return m_texture_handle.get();
}

void SDL::Render::DirectTextureFactory::render(SDL_Renderer* renderer_handle, SDL_Rect* source_rect, SDL_Rect* destination_rect)
{
	if (m_texture_handle.get() == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Attempted to render null texure (SDL::Render::DirectTextureWrapper::render)");
		return;
	}

	SDL_RenderCopy(renderer_handle, m_texture_handle.get(), source_rect, destination_rect);
}

void SDL::Render::DirectTextureFactory::blit_texture(SDL_Renderer* renderer_handle, SDL::Render::DirectTextureFactory& blitted_texture, SDL_Rect* source_rect, SDL_Rect* destination_rect)
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::Render::DirectTextureFactory::blit_texture [%p]", &SDL::Render::DirectTextureFactory::blit_texture);

	if (m_texture_handle.get() == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\t--- Attempted to blit image to the null texture");
		return;
	}

	if (blitted_texture.get() == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\t--- Attempted to blit null image to the texture");
		return;
	}

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Reseting current processed texture blend mode and the render target");
	SDL_SetTextureBlendMode(m_texture_handle.get(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderer_handle, m_texture_handle.get());

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Blending the textures");
	SDL_RenderCopy(renderer_handle, blitted_texture.get(), source_rect, destination_rect);

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Reseting the render target");
	SDL_SetRenderTarget(renderer_handle, NULL);
}

void SDL::Render::DirectTextureFactory::load_image(SDL_Renderer* renderer_handle, const std::string& image_path)
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::Render::DirectTextureFactory::load_image [%p]", &SDL::Render::DirectTextureFactory::load_image);

	SDL_Surface* image_surface;
	SDL_Texture* image_texture;

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Loading image %s", image_path.c_str());
	if ((image_surface = IMG_Load(image_path.c_str())) == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- Image loading failed");
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", IMG_GetError());
		return;
	}

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Serializing image to the texture");
	if ((image_texture = SDL_CreateTextureFromSurface(renderer_handle, image_surface)) == NULL)
	{
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- Image convertation to texture failed");
		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", IMG_GetError());
		return;
	}
	
	if (m_texture_handle.get() == NULL)
	{
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Attempted to reasign the texture, replacing the previous with the new one");
		SDL_DestroyTexture(m_texture_handle.get());
	}

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Updating the texture handler");
	m_texture_handle.reset(image_texture);
	SDL_FreeSurface(image_surface);
}
