#include "DirectTextureContainer.h"
#include "DirectTextureFactory.h"

static constexpr const std::string_view default_texture_airbag("data/assets/undefined.png");

[[maybe_unused]] void SDL::DirectTextureContainer::render(SDL::DirectRendererHandle renderer_handle) const
{
    SDL::SharedTextureRect *texture_preprocessed_source;
    SDL::SharedTextureRect *texture_preprocessed_destination;

    // Compare the values of the two direct structs
    auto compare_direct_structs = [](const SDL::SharedTextureRect& s1, const SDL::SharedTextureRect& s2) -> bool {
        return std::tie(s1.x, s1.y, s1.w, s1.h) == std::tie(s2.x, s2.y, s2.w, s2.h);
    };

    // Validate the user-given renderer handle
    if(renderer_handle == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "\t--- The provided renderer is NULL, unable to render the textures");
        throw SDL::DirectInvalidArgument();
    }

    // Check if the source rectangle is invalid(or not provided by the user)
    if(compare_direct_structs(m_texture_source, SDL::NULL_RECT))
        texture_preprocessed_source = nullptr;
    else
        texture_preprocessed_source = const_cast<SDL::SharedTextureRect *>(&m_texture_source);

    // Check if the destination rectangle is invalid(or not provided by the user)
    if(compare_direct_structs(m_texture_destination, SDL::NULL_RECT))
        texture_preprocessed_destination = nullptr;
    else
        texture_preprocessed_destination = const_cast<SDL::SharedTextureRect *>(&m_texture_destination);

    // If the texture is enabled(means it holds some data, and do not currently released)
    if(m_direct_object_state == SDL::DirectObjectState::Enabled)
        SDL_RenderCopy(renderer_handle, m_shared_texture.get(), texture_preprocessed_source, texture_preprocessed_destination);
}

[[maybe_unused]] void SDL::DirectTextureContainer::enable(SDL::DirectRendererHandle renderer_handle)
{
    // Validate the user-given renderer handle
    if(renderer_handle == nullptr) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "\t--- The provided renderer is NULL, unable to enable the textures");
        throw SDL::DirectInvalidArgument();
    }

    // If the object is already enabled, we should either disable and then enable it,
    // or do nothing
    if(m_direct_object_state == SDL::DirectObjectState::Enabled) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- The texture %s is already enabled", m_direct_object_name.c_str());
        return;
    }

    // The object is disabled but the texture container stores something, it means
    // that the object is initialized with the foreign texture data(because texture
    // cannot just `dont't free` simultaneously
    if(m_shared_texture != nullptr) {
        m_direct_object_state = SDL::DirectObjectState::Enabled;
        return;
    }

    // Load the texture from image, and wrap its pointer into the SharedDirectTexture(typedef) container
    //
    // If we're unable to load the texture airbag, there's no texture going to be appearing on the screen,
    // so we're making a maybe bad design choice to raise an error, and stop the program
    m_shared_texture = SDL::SharedDirectTexture(SDL::DirectTextureFactory::load_texture(
            renderer_handle,
            m_direct_object_name,
            default_texture_airbag
    ), SDL_DestroyTexture);


    // The code did not throw any exception at this time, so we are free
    // to set the state to `Enabled` here, and avoid code repetition
    //
    // Set the texture state to enabled(so it can be rendered)
    m_direct_object_state = SDL::DirectObjectState::Enabled;
}

[[maybe_unused]] void SDL::DirectTextureContainer::disable()
{
    if(m_direct_object_state == SDL::DirectObjectState::Disabled) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Texture %s is already disabled", m_direct_object_name.c_str());
        return;
    }

    // Destroy the texture internal data
    SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Unloading texture %s", m_direct_object_name.c_str());
    SDL_DestroyTexture(m_shared_texture.get());

    // Disable the texture(do not render it)
    m_direct_object_state = SDL::DirectObjectState::Disabled;
}