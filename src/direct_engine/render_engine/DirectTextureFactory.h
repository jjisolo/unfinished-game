#pragma once

#include <RenderingEngine.h>
#include <RenderingExceptions.h>

#include <utility>

namespace SDL
{
	class DirectTextureFactory
	{
    public:

        /*!
         * \brief DirectTextureFactory class constructor
         *
         * \param texture_airbag The texture that is loaded when some other texture
         * load fails.
        */
        explicit DirectTextureFactory(std::string texture_airbag = "undefined.png") :
            m_texture_airbag(std::move(texture_airbag))
        {

        }

        /*!
         *  \brief Get the texture airbag name
         */
        void inline set_texture_airbag(const std::string& airbag)
        {
            m_texture_airbag = airbag;
        }

        /*!
         *  \brief Set the texture airbag name
         */
        std::string inline get_texture_airbag()
        {
            return m_texture_airbag;
        }

		/*!
		 * \brief Load the image from given relative path
		 *
		 * \param renderer_handle A handle to an SDL2 renderer
		 * \param image_path      A path to an image
		 */
		 SDL_Texture* create_texture_from_image(SDL_Renderer* renderer_handle, const std::string& image_path);

    private:
        std::string m_texture_airbag;
	};
}