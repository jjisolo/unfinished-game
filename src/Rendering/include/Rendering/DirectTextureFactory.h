#pragma once

#include <RenderingEngine.h>
#include <RenderingExceptions.h>
#include <DirectTextureContainer.h>

namespace SDL
{
    using DirectTextureHandle               = SDL_Texture  *;
    using DirectSurfaceHandle               = SDL_Surface  *;
    using DirectRendererHandle              = SDL_Renderer *;
    using DirectFontHandle                  = TTF_Font     *;

    using DirectFontTextContainer           = const std::string_view&;
    using DirectFontDisplaySize             = std::size_t;
    using DirectFontRelativePath            = std::string;

    using DirectColor                       = SDL_Color;

    enum class DirectFontRenderKind : std::uint8_t
    {
        Blended        [[maybe_unused]],
        Shaded         [[maybe_unused]],
        Solid          [[maybe_unused]],
        BlendedWrapped [[maybe_unused]],
        ShadedWrapped  [[maybe_unused]],
        SolidWrapped   [[maybe_unused]],
    };

    // From the Google Roboto fonts types
    enum class DirectFontDisplayVariant : std::uint8_t
    {
        Normal               [[maybe_unused]],
        Bold                 [[maybe_unused]],
        Italic               [[maybe_unused]],
        Underline            [[maybe_unused]],
        StrikeThrough        [[maybe_unused]],
        BoldItalic           [[maybe_unused]],
        UnderlineItalic      [[maybe_unused]],
        StrikeThroughItalic  [[maybe_unused]],
    };

	class DirectTextureFactory
    {
    public:
        /*!
         * \brief Load the image from given relative path
         *
         * \param renderer_handle A handle to an SDL2 renderer
         * \param image_path      A path to the image
         * \param image_airbag    A path to the image airbag(image that loads if the
         * primary image loading failed)
         */
        [[maybe_unused]] static SDL::DirectTextureHandle load_texture(
                SDL::DirectRendererHandle  renderer_handle,
                const std::string_view    &image_path,
                const std::string_view    &image_airbag)
        {
            if(renderer_handle == nullptr) {
                spdlog::error("Cannot create direct texture: the renderer is nullptr");
                throw SDL::DirectInvalidArgument();
            }

            if(image_path.empty()) {
                spdlog::error("Cannot create direct texture: image_path is invalid");
                throw SDL::DirectInvalidArgument();
            }

            if(image_airbag.empty()) {
                spdlog::error("Cannot create direct texture: image_airbag is invalid");
                throw SDL::DirectInvalidArgument();
            }

            SDL_Texture* image_texture;

            auto load_direct_image = [](SDL::DirectRendererHandle renderer_handle, const char* texture_path)
            {
                SDL_Surface* image_surface;
                SDL_Texture* image_texture;

                image_surface = IMG_Load(texture_path);
                if(image_surface == nullptr) {
                    spdlog::error("{}", IMG_GetError());
                    throw SDL::DirectSystemException();
                }

                image_texture = SDL_CreateTextureFromSurface(renderer_handle, image_surface);
                if(image_texture == nullptr) {
                    spdlog::error("{}", IMG_GetError());
                    throw SDL::DirectSystemException();
                }

                spdlog::debug("Loaded texture {}", texture_path);
                SDL_FreeSurface(image_surface);

                return image_texture;
            };

            try
            {
                image_texture = load_direct_image(renderer_handle, std::string(image_path).c_str());
            }
            catch(SDL::DirectSystemException &exception)
            {
                image_texture = load_direct_image(renderer_handle, std::string(image_airbag).c_str());
            }

            return image_texture;
        }

        [[maybe_unused]] static SDL::DirectTextureHandle load_font(
                SDL::DirectRendererHandle    renderer,
                SDL::DirectFontHandle        font,
                SDL::DirectFontRenderKind    kind     = SDL::DirectFontRenderKind::Blended,
                SDL::DirectFontTextContainer text     = "Sample text",
                SDL::DirectColor             color_fg = {255, 255, 255, 255},
                SDL::DirectColor             color_bg = {0,   0,   0,   255})
        {
            SDL::DirectSurfaceHandle text_surface;
            SDL::DirectTextureHandle text_texture;

            switch (kind) {
                case SDL::DirectFontRenderKind::Blended:
                    text_surface = TTF_RenderText_Blended(font, std::string(text).c_str(), color_fg);
                    break;
                case SDL::DirectFontRenderKind::Shaded:
                    text_surface = TTF_RenderText_Shaded (font, std::string(text).c_str(), color_fg, color_bg);
                    break;
                case SDL::DirectFontRenderKind::Solid:
                    text_surface = TTF_RenderText_Solid  (font, std::string(text).c_str(), color_fg);
                    break;
                case SDL::DirectFontRenderKind::BlendedWrapped:
                    text_surface = TTF_RenderText_Blended_Wrapped(font, std::string(text).c_str(), color_fg, 0);
                    break;
                case SDL::DirectFontRenderKind::ShadedWrapped:
                    text_surface = TTF_RenderText_Shaded_Wrapped(font, std::string(text).c_str(), color_fg, color_bg, 0);
                    break;
                case SDL::DirectFontRenderKind::SolidWrapped:
                    text_surface = TTF_RenderText_Solid_Wrapped(font, std::string(text).c_str(), color_fg, 0);
                    break;
                default:
                    text_surface = TTF_RenderText_Blended(font, std::string(text).c_str(), color_fg);
                    break;
            }

            if(text_surface == nullptr) {
                spdlog::error("{}", TTF_GetError());
                throw SDL::DirectTrueTypeSystemException();
            }

            text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

            if(text_texture == nullptr) {
                spdlog::error("{}", SDL_GetError());
                throw SDL::DirectSystemException();
            }

            SDL_FreeSurface(text_surface);
            return text_texture;
        }
    };
}