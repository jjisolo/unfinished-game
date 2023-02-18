#pragma once

#include <Rendering/DirectObjectContainerPreset.h>
#include <Rendering/DirectTextureFactory.h>
#include <Rendering/RenderingEngine.h>

#include <utility>

// TODO: more SharedTextureLoadVariant - copy, load_from_image, load_from_surface, convert_from_font, convert_from_ui_object(?) ...
namespace SDL
{
    using DirectTextureHandle  = SDL_Texture *;

    using SharedDirectTexture  = std::shared_ptr<SDL_Texture>;
    using SharedTexturePath    = std::string;
    using SharedTextureRect    = SDL_Rect;

    class DirectTextureContainer: public SDL::priv::DirectObjectContainerPreset
    {
        friend class DirectTextureFactory;
        friend class RenderScene;
    public:

        /*!
        * \brief TrueType font texture constructor
        *
        * \param texture_handle A handle to the texture
        * \param font_name Name of the font(used for debugging)
        * \param font_source Source rectangle of the texture
        * \param font_destination Destination rectangle of the texture
        *
        * \throws DirectInvalidArgument If the texture handle is invalid
        *
        * \sa SDL::DirectTextureHandle
        * \sa SDL::SharedTexturePath
        * \sa SDL::SharedTextureRect
        */
        DirectTextureContainer(
                SDL::DirectTextureHandle texture_handle,
                SDL::SharedTexturePath   font_name,
                SDL::SharedTextureRect   font_source,
                SDL::SharedTextureRect   font_destination)
                :
                SDL::priv::DirectObjectContainerPreset{font_name, SDL::DirectObjectState::Disabled },
                m_texture_source                      {font_source                                 },
                m_texture_destination                 {font_destination                            }
        {
            // Assert the texture handle, it should not be NULL
            if(texture_handle == nullptr) {
                spdlog::error("The texture {} data is invalid", std::string(font_name).c_str());
                throw SDL::DirectInvalidArgument();
            }

            // Log this!
            spdlog::debug("Loaded texture {} (baked from the font)", font_name.c_str());

            // Create shared texture pointer
            m_shared_texture = SDL::SharedDirectTexture(texture_handle, SDL_DestroyTexture);
        }

        /*!
         * \brief Image texture constructor
         *
         * \param texture_path Path to the image(recursive)
         * \param texture_source Source rectangle of the image
         * \param texture_destination Destination rectangle of the image
         *
         * \sa SDL::SharedTexturePath
         * \sa SDL::SharedTextureRect
         */
        DirectTextureContainer(
                SDL::SharedTexturePath                      texture_path,
                SDL::SharedTextureRect                      texture_source,
                SDL::SharedTextureRect                      texture_destination)
                :
                SDL::priv::DirectObjectContainerPreset{std::move(texture_path), SDL::DirectObjectState::Disabled},
                m_texture_source                      {texture_source         },
                m_texture_destination                 {texture_destination    }
        {
        }

        /*!
         * \brief Render the texture on the screen
         *
         * \param renderer_handle A handle to the created renderer instance
         *
         * \attention The texture should be enabled, or it won't
         * be rendered! Consider using enable() function
         *
         * \throws SDL::DirectInvalidArgument If the renderer handle is
         * invalid
         *
         * \sa SDL::DirectTextureContainer::enable
         * \sa SDL::DirectInvalidArgument
         * \sa SDL::DirectRendererHandle
         */
        [[maybe_unused]] void render(SDL::DirectRendererHandle renderer_handle) const final;

        /*!
         * \brief Load the texture
         *
         * Load the texture into the memory
         *
         * \attention To render the texture the enable() method should be
         * called
         *
         * \attention If the texture container is constructed using foreign
         * texture data, the enable() function should be called
         * whatever
         *
         * \warning Use disable() function after the enable()
         * to release the texture data
         *
         * \throws DirectInvalidArgument if the provided texture
         * data(renderer handle, path, airbag path, renderer handle are invalid
         *
         * \throws DirectSystemException if the texture could not
         * be loaded using SDL_ttf library backend, or the texture
         * convertion from the surface failed
         *
         * \sa SDL::DirectTextureFactory::load_texture()
         * \sa SDL::DirectRendererHandle
         */
        [[maybe_unused]] void enable(SDL::DirectRendererHandle renderer_handle) final;

        /*!
         * \brief Unload the texture
         *
         * Unload the texture from the memory, keep it
         * launch information stored though
         */
        [[maybe_unused]] void disable() final;

    private:
        SDL::SharedDirectTexture m_shared_texture;         /*! A shared pointer of the texture */
        SDL::SharedTextureRect   m_texture_source;         /*! Source rect of the texture */
        SDL::SharedTextureRect   m_texture_destination;    /*! Destination rect of the texture */
    };
}