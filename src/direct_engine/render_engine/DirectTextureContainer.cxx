#include <DirectTextureContainer.h>

[[maybe_unused]] void SDL::DirectTextureContainer::render(SDL::DirectRendererHandle renderer_handle) const
{
    // If the texture is enabled
    if(m_direct_object_state == SDL::DirectObjectState::Enabled)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t --- Rendering the texture %s", m_shared_texture_path.c_str());
        SDL_RenderCopy(renderer_handle, m_shared_texture.get(), &m_texture_source, &m_texture_destination);
    }
}

[[maybe_unused]] void SDL::DirectTextureContainer::enable(SDL::DirectRendererHandle renderer_handle)
{
    // We do not use logging here because we have full texture loading logging under the
    // DirectTextureFactory class
    switch(m_shared_texture_load_variant)
    {
        // The class is the image container that works directly with an SDL_Image library
        case SDL::SharedTextureLoadVariant::LoadImageByPath:
        {
            // Load the texture from image, and wrap its pointer into the SharedDirectTexture(typedef) container
            m_shared_texture = SDL::SharedDirectTexture(m_binded_texture_factory.create_texture_from_image(
                renderer_handle,
                m_shared_texture_path
            ), SDL_DestroyTexture);

            // Set the texture state to enabled(so it can be rendered)
            m_direct_object_state = SDL::DirectObjectState::Enabled;
            break;
        }
        // Somehow the load variant is not specified, so we are loading the airbag. Just in case
        default:
        {
            // Load the airbag from image, and wrap its pointer into the SharedDirectTexture(typedef) container
            m_shared_texture = SDL::SharedDirectTexture(m_binded_texture_factory.create_texture_from_image(
                    renderer_handle,
                    m_binded_texture_factory.get_texture_airbag()
            ), SDL_DestroyTexture);

            // Set the texture state to enabled(so it can be rendered)
            m_direct_object_state = SDL::DirectObjectState::Enabled;
            break;
        }
    }
}

[[maybe_unused]] void SDL::DirectTextureContainer::disable()
{
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t --- Unloading texture %s", m_shared_texture_path.c_str());

    // Disable the texture(do not render it)
    m_direct_object_state = SDL::DirectObjectState::Disabled;

    // Destroy the texture internal data
    SDL_DestroyTexture(m_shared_texture.get());
}