#pragma once

#include <DirectObjectContainerPreset.h>
#include <DirectTextureFactory.h>
#include <RenderingEngine.h>

#include <utility>

namespace SDL
{
    using SharedDirectTexture  = std::shared_ptr<SDL_Texture>;
    using SharedTexturePath    = std::string;
    using SharedTextureName    = std::string;
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
         * \param renderer   A handle(pointer) to the SDL_Renderer instance
         * \param factory    A reference to an existing SDL::DirectTextureFactory instance
         * \param image_path Path to an image(recursive)
         * \param image_source Source rectangle of an image
         * \param image_destination Destination rectangle of an image
         *
         * \sa  SDL::EngineInterface::get_renderer_handle()
         * \sa  SDL::DirectTextureFactory
         * \sa  SDL::DirectRendererHandle
         */
        DirectTextureContainer(
                SDL::DirectTextureFactory& factory,
                SDL::SharedTexturePath     image_path,
                SDL::SharedTextureRect     image_source,
                SDL::SharedTextureRect     image_destination)
                :
                SDL::priv::DirectObjectContainerPreset{SDL::DirectObjectState::Disabled},
                m_binded_texture_factory              {factory                         },
                m_shared_texture_path                 {std::move(image_path)           },
                m_texture_source                      {image_source                    },
                m_texture_destination                 {image_destination               }
        {
            // Using this constructor is already defines the texture load type as an SharedTextureLoadVariant::LoadImageByPath,
            // so we don't need explicitly pass the state to the constructor
            m_shared_texture_load_variant = SharedTextureLoadVariant::LoadImageByPath;
        }

        /*!
         * \brief Render the texture on the screen
         */
        [[maybe_unused]] virtual void render(SDL::DirectRendererHandle renderer_handle) const final;

        /*!
         * \brief Load the texture
         */
        [[maybe_unused]] virtual void enable(SDL::DirectRendererHandle renderer_handle) final;

        /*!
         * \brief Unload the texture
         */
        [[maybe_unused]] virtual void disable() final;


    private:
        SDL::DirectTextureFactory&    m_binded_texture_factory;

        SDL::SharedDirectTexture      m_shared_texture;
        SDL::SharedTextureLoadVariant m_shared_texture_load_variant;
        SDL::SharedTexturePath        m_shared_texture_path;

        SDL::SharedTextureName m_shared_texture_name;
        SDL::SharedTextureRect m_texture_source;
        SDL::SharedTextureRect m_texture_destination;
    };
}