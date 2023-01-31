#pragma once

#include <RenderingEngine.h>
#include <RenderingExceptions.h>
#include <DirectTextureContainer.h>

namespace SDL
{
    using DirectFontDisplaySize             = std::size_t;
    using DirectFontRelativePath            = std::string;
    using DirectTextureFactoryAirbagTag     = std::string;

    // From the Google Roboto fonts types
    enum class DirectFontDisplayVariant : std::uint8_t
    {
        Black          [[maybe_unused]],
        Bold           [[maybe_unused]],
        Light          [[maybe_unused]],
        Italic         [[maybe_unused]],
        Medium         [[maybe_unused]],
        Thin           [[maybe_unused]],
        BlackItalic    [[maybe_unused]],
        BoldItalic     [[maybe_unused]],
        LightItalic    [[maybe_unused]],
        ItalicItalic   [[maybe_unused]],
        MediumItalic   [[maybe_unused]],
        ThinItalic     [[maybe_unused]],
    };

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
        [[maybe_unused]] void inline set_texture_airbag(const std::string& airbag)
        {
            m_texture_airbag = airbag;
        }

        /*!
         *  \brief Set the texture airbag name
         */
        [[maybe_unused]] std::string inline get_texture_airbag()
        {
            return m_texture_airbag;
        }

		/*!
		 * \brief Load the image from given relative path
		 *
		 * \param renderer_handle A handle to an SDL2 renderer
		 * \param image_path      A path to an image
		 */
        [[maybe_unused]] SDL_Texture* load_texture(SDL_Renderer* renderer_handle, const std::string& image_path);

        /*!
         * \brief Load the font from given relative path
         *
         * \param renderer_handle A handle to an SDL2 renderer
         * \param image_path      A path to the font
         */
        [[maybe_unused]] SDL_Texture* load_font(
                SDL::DirectRendererHandle     renderer_handle,
                const std::string&            font_path,
                SDL::DirectFontDisplayVariant font_variant,
                SDL::DirectFontDisplaySize    font_size
        );

    private:
        SDL::DirectTextureFactoryAirbagTag     m_texture_airbag;
	};
}