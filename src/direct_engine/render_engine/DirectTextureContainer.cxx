#include <DirectTextureContainer.h>

[[maybe_unused]] void SDL::DirectTextureContainer::render(SDL::DirectRendererHandle renderer_handle) const
{
    // If the texture is enabled
    if(m_direct_object_state == SDL::DirectObjectState::Enabled)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t --- Rendering the texture %s", m_direct_object_name.c_str());
        SDL_RenderCopy(renderer_handle, m_shared_texture.get(), &m_texture_source, &m_texture_destination);
    }
}

[[maybe_unused]] void SDL::DirectTextureContainer::enable(SDL::DirectRendererHandle renderer_handle, std::shared_ptr<SDL::DirectTextureFactory> direct_factory)
{
    // If the object is already enabled, we should either disable and then enable it,
    // or do nothing
    if(m_direct_object_state == SDL::DirectObjectState::Enabled)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- The texture %s is already enabled", m_direct_object_name.c_str());
        return;
    }

    // We do not use logging here because we have full texture loading logging under the
    // DirectTextureFactory class
    switch(m_shared_texture_load_variant)
    {
        // The class is the image container that works directly with an SDL_Image library
        case SDL::SharedTextureLoadVariant::LoadImageByPath:
        {
            // Load the texture from image, and wrap its pointer into the SharedDirectTexture(typedef) container
            //
            // If we're unable to load the texture airbag, there's no texture going to be appearing on the screen,
            // so we're making a maybe bad design choice to raise an error, and stop the program
            m_shared_texture = SDL::SharedDirectTexture(direct_factory->create_texture_from_image(
                renderer_handle,
                m_direct_object_name
            ), SDL_DestroyTexture);

            break;
        }
        // Somehow the load variant is not specified, so we are loading the airbag. Just in case
        default:
        {
            // Load the airbag from image, and wrap its pointer into the SharedDirectTexture(typedef) container
            //
            // Actually we're loading the airbag twice(if it does not exist), but who cares about the performance,
            // when the internal structure of the render_object is broken
            m_shared_texture = SDL::SharedDirectTexture(direct_factory->create_texture_from_image(
                    renderer_handle,
                    direct_factory->get_texture_airbag()
            ), SDL_DestroyTexture);

            break;
        }
    }

    // The code did not throw any exception at this time, so we are free
    // to set the state to `Enabled` here, and avoid code repetition
    //
    // Set the texture state to enabled(so it can be rendered)
    m_direct_object_state = SDL::DirectObjectState::Enabled;
}

[[maybe_unused]] void SDL::DirectTextureContainer::disable()
{
    if(m_direct_object_state == SDL::DirectObjectState::Disabled)
    {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Texture %s is already disabled", m_direct_object_name.c_str());
        return;
    }

    // Destroy the texture internal data
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Unloading texture %s", m_direct_object_name.c_str());
    SDL_DestroyTexture(m_shared_texture.get());

    // Disable the texture(do not render it)
    m_direct_object_state = SDL::DirectObjectState::Disabled;
}