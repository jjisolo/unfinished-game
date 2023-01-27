#pragma once

#include <DirectObjectContainerPreset.h>
#include <DirectTextureFactory.h>
#include <RenderingEngine.h>

#include <utility>

// TODO: more SharedTextureLoadVariant - copy, load_from_image, load_from_surface, convert_from_font, convert_from_ui_object(?) ...
namespace SDL
{
    using SharedDirectTexture  = std::shared_ptr<SDL_Texture>;
    using SharedTexturePath    = std::string;
    using SharedTextureRect    = SDL_Rect;

    enum class SharedTextureLoadVariant : std::uint8_t
    {
        Copy,
        LoadImageByPath
    };

    class DirectTextureContainer: public SDL::priv::DirectObjectContainerPreset
    {
        friend class DirectTextureFactory;
        friend class RenderScene;
    public:

        /*!
         * \brief Image texture constructor
         *
         * \param image_path Path to an image(recursive)
         * \param image_source Source rectangle of an image
         * \param image_destination Destination rectangle of an image
         *
         */
        DirectTextureContainer(
                SDL::SharedTexturePath                      image_path,
                SDL::SharedTextureRect                      image_source,
                SDL::SharedTextureRect                      image_destination)
                :
                SDL::priv::DirectObjectContainerPreset{std::move(image_path), SDL::DirectObjectState::Disabled},
                m_texture_source                      {image_source         },
                m_texture_destination                 {image_destination    }
        {
            // Using this constructor is already defines the texture load type as an SharedTextureLoadVariant::LoadImageByPath,
            // so we don't need explicitly pass the state to the constructor
            m_shared_texture_load_variant = SharedTextureLoadVariant::LoadImageByPath;
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
        SDL::SharedDirectTexture      m_shared_texture;
        SDL::SharedTextureLoadVariant m_shared_texture_load_variant;

        SDL::SharedTextureRect m_texture_source;
        SDL::SharedTextureRect m_texture_destination;
    };
}