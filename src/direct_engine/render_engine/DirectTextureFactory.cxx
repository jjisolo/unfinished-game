#include <DirectTextureFactory.h>

SDL_Texture* SDL::DirectTextureFactory::create_texture_from_image(SDL_Renderer* renderer_handle, const std::string& image_path)
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::DirectTextureFactory::load_image [%p]", &SDL::DirectTextureFactory::create_texture_from_image);

    SDL_Texture* image_texture;

    auto load_direct_image = [this](SDL_Renderer* renderer_handle, const char* texture_path)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Loading image %s", texture_path);
        SDL_Surface* image_surface;
        SDL_Texture* image_texture;

        image_surface = IMG_Load(texture_path);
        if(image_surface == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- Image loading failed");
            SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", IMG_GetError());
            throw SDL::DirectSystemException();
        }

        image_texture = SDL_CreateTextureFromSurface(renderer_handle, image_surface);
        if(image_texture == NULL)
        {
            SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- Image convertation to texture failed");
            SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "\t--- The following error message is: %s", IMG_GetError());
            throw SDL::DirectSystemException();
        }

        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Cleaning up the image surface");
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