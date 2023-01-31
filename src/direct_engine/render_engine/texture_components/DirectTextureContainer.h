#pragma once

#include <DirectObjectContainerPreset.h>
#include "DirectTextureFactory.h"
#include "RenderingEngine.h"

#include <utility>

// TODO: more SharedTextureLoadVariant - copy, load_from_image, load_from_surface, convert_from_font, convert_from_ui_object(?) ...
namespace SDL
{
    using SharedDirectTexture  = std::shared_ptr<SDL_Texture>;
    using SharedTexturePath    = std::string;
    using SharedTextureRect    = SDL_Rect;

    enum class DirectTextureContainerLoadVariant : std::uint8_t
    {
        Copy,
        LoadImage,
        LoadFont,
    };

    class DirectTextureContainer: public SDL::priv::DirectObjectContainerPreset
    {
        friend class DirectTextureFactory;
        friend class RenderScene;
    public:

        /*!
         * \brief TrueType font texture constructor
         *
         * \param font_path Path to the font(recursive)
         * \param font_source Source rectangle of the font
         * \param font_destination Destination rectangle of the font
         *
         */
        DirectTextureContainer(
                DirectTextureContainerLoadVariant           texture_load_variant,
                SDL::SharedTexturePath                      texture_path,
                SDL::SharedTextureRect                      texture_source,
                SDL::SharedTextureRect                      texture_destination)
                :
                SDL::priv::DirectObjectContainerPreset{std::move(texture_path), SDL::DirectObjectState::Disabled},
                m_shared_texture_load_variant         {texture_load_variant   },
                m_texture_source                      {texture_source         },
                m_texture_destination                 {texture_destination    }
        {
        }

        /*!
         * \brief Render the texture on the screen
         */
        [[maybe_unused]] void render(SDL::DirectRendererHandle renderer_handle) const final;

        /*!
         * \brief Load the texture
         */
        [[maybe_unused]] void enable(SDL::DirectRendererHandle renderer_handle, std::shared_ptr<SDL::DirectTextureFactory> direct_factory) final;

        /*!
         * \brief Unload the texture
         */
        [[maybe_unused]] void disable() final;

    private:
        SDL::SharedDirectTexture               m_shared_texture;
        SDL::DirectTextureContainerLoadVariant m_shared_texture_load_variant;

        SDL::SharedTextureRect m_texture_source;
        SDL::SharedTextureRect m_texture_destination;
    };
}