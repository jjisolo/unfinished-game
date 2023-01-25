#include <DirectTextureContainer.h>


void SDL::DirectTextureContainer::enable()
{
    switch(m_shared_texture_load_variant)
    {
        // The class is the image container that works directly with an SDL_Image library
        case SDL::SharedTextureLoadVariant::LoadImageByPath:
        {
            m_shared_texture = SDL::SharedDirectTexture(m_binded_texture_factory.create_texture_from_image(
                m_binded_renderer_handle,
                m_shared_texture_path
            ), SDL_DestroyTexture);
            break;
        }
        // Somehow the load variant is not specified, so we are loading the airbag. Just in case
        default:
        {
            m_shared_texture = SDL::SharedDirectTexture(m_binded_texture_factory.create_texture_from_image(
                    m_binded_renderer_handle,
                    m_binded_texture_factory.get_texture_airbag()
            ), SDL_DestroyTexture);
            break;
        }
    }
}

void SDL::DirectTextureContainer::disable()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t --- Unloading texture %s", m_shared_texture_path.c_str());


}