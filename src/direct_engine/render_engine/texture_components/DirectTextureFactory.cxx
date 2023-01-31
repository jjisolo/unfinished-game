#include "DirectTextureFactory.h"

SDL_Texture* SDL::DirectTextureFactory::load_texture(SDL_Renderer* renderer_handle, const std::string& image_path)
{
    SDL_Texture* image_texture;

    auto load_direct_image = [](SDL_Renderer* renderer_handle, const char* texture_path)
    {
        SDL_Surface* image_surface;
        SDL_Texture* image_texture;

        image_surface = IMG_Load(texture_path);
        if(image_surface == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- Image %s loading failed",            texture_path);
            SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", IMG_GetError());
            throw SDL::DirectSystemException();
        }

        image_texture = SDL_CreateTextureFromSurface(renderer_handle, image_surface);
        if(image_texture == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- Image %s convertation to texture failed", texture_path);
            SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s",      IMG_GetError());
            throw SDL::DirectSystemException();
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Loaded the texture %s", texture_path);
        SDL_FreeSurface(image_surface);

        return image_texture;
    };

    try
    {
        image_texture = load_direct_image(renderer_handle, image_path.c_str());
    }
    catch(SDL::DirectSystemException &exception)
    {
        image_texture = load_direct_image(renderer_handle, m_texture_airbag.c_str());
    }

    return image_texture;
}

SDL_Texture *SDL::DirectTextureFactory::load_font(SDL::DirectRendererHandle     renderer_handle,
                                                                  const std::string            &font_path,
                                                                  SDL::DirectFontDisplayVariant font_variant,
                                                                  SDL::DirectFontDisplaySize    font_size)
{
    return nullptr;
}
