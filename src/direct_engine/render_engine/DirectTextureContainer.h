#pragma once

#include <DirectTextureFactory.h>
#include <RenderScene.h>

#include <utility>

namespace SDL
{
    using DirectRendererHandle = SDL_Renderer*;

    using SharedDirectTexture  = std::shared_ptr<SDL_Texture>;
    using SharedTexturePath    = std::string;
    using SharedTextureName    = std::string;
    using SharedTextureRect    = SDL_Rect;

    enum class SharedTextureLoadVariant : std::uint8_t
    {
        Copy,
        LoadImageByPath
    };

    enum class SharedTextureState : std::uint8_t
    {
        Enabled,
        Disabled
    };

    class DirectTextureContainer
    {
        friend class SDL::DirectTextureFactory;
        friend class SDL::RenderScene;
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
         */
        DirectTextureContainer(
                SDL::DirectRendererHandle& renderer,
                SDL::DirectTextureFactory& factory,
                SDL::SharedTexturePath image_path,
                SDL::SharedTextureRect image_source,
                SDL::SharedTextureRect image_destination)
                :
                m_binded_renderer_handle(renderer),
                m_binded_texture_factory(factory),
                m_shared_texture_path   (std::move(image_path)),
                m_texture_source        (image_source),
                m_texture_destination   (image_destination)
            {
                m_shared_texture_load_variant = SharedTextureLoadVariant::LoadImageByPath;
                m_shared_texture_state        = SharedTextureState::Disabled;
            }

        /*!
         * \brief Load the texture
         */
        void enable();

        /*!
         * \brief Unload the texture
         */
        void disable();

        /*!
             * \brief Get the texture state(is it loaded or its not)
         */
        inline SDL::SharedTextureState get_state() const noexcept
        {
            return m_shared_texture_state;
        }



    private:
        SDL::DirectTextureFactory&    m_binded_texture_factory;
        SDL::DirectRendererHandle&    m_binded_renderer_handle;

        SDL::SharedTextureName        m_shared_texture_name;

        SDL::SharedDirectTexture      m_shared_texture;
        SDL::SharedTextureState       m_shared_texture_state;
        SDL::SharedTextureLoadVariant m_shared_texture_load_variant;
        SDL::SharedTexturePath        m_shared_texture_path;

        SDL::SharedTextureRect m_texture_source;
        SDL::SharedTextureRect m_texture_destination;
    };

}

