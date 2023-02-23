#pragma once

#include <Rendering/RenderingEngine.h>
#include <Rendering/RenderingExceptions.h>
#include <Rendering/DirectTextureContainer.h>

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
                const std::string_view    &image_airbag,
                const bool                 load_airbag = false)
        {
            // If the given renderer handle is invalid it means that something wrong alot
            // higher in sense of abstractions, so we are raise the exception
            if(renderer_handle == nullptr) {
                spdlog::error("Cannot create direct texture: the renderer is nullptr");
                throw SDL::DirectInvalidArgument();
            }
            
            // Assert texture name invalid variants
            auto assert_texture_path_invariants = [](std::string texture_path) -> bool { 
                if(texture_path.empty()) return false;

                // Check if the provided string ends with given value
                auto string_endswith = [](const std::string& str, const std::string& ending) -> bool {
                    if(ending.size() > str.size()) return false;
                    return std::equal(ending.rbegin(), ending.rend(), ending.rbegin());
                };
                
                // Due to the SDL2_image wiki the supported formats are:
                // BMP, GIF, JPEG, LBM, PCX, PNG, PNM (PPM/PGM/PBM), QOI, TGA, XCF, XPM, SVG
                //
                // So we just correlate this formats to the given string
                #undef  _CheckImageFormat
                #define _CheckImageFormat(ending) if(string_endswith(texture_path, (ending))) return true
                _CheckImageFormat("JPEG");
                _CheckImageFormat("BMP");
                _CheckImageFormat("GIF");
                _CheckImageFormat("JPG");
                _CheckImageFormat("LBM");
                _CheckImageFormat("PCX");
                _CheckImageFormat("PNG");
                _CheckImageFormat("PNM");
                _CheckImageFormat("QOI");
                _CheckImageFormat("TGA");
                _CheckImageFormat("XCF");
                _CheckImageFormat("XPM");
                _CheckImageFormat("SVG");
                #undef _CheckImageFormat

                return false;
            };

            // Image path and image airbag validation
            bool image_path_is_valid = assert_texture_path_invariants(std::string(image_path));
            bool airbg_path_is_valid = assert_texture_path_invariants(std::string(image_airbag));
            
            // Both provided images are invalid
            if(!image_path_is_valid && !airbg_path_is_valid) {
                spdlog::error("Cannot create direct texture: image_path and image_airbag is invalid");
                throw SDL::DirectInvalidArgument(); 
            }
            
            // The image path is valid, but image airbag is not(possible error)
            if(!image_path_is_valid && airbg_path_is_valid) {
                spdlog::warn("Provided image path is not valid", image_path);
            }

            // The image path is not valid, so we load the airbag
            if(!image_path_is_valid && airbg_path_is_valid) {
                spdlog::warn("Image path for {} is invalid, loading its airbag", image_path);
            }

            // When the load airbag directly flag is set, and airbag is invalid
            if(load_airbag && !airbg_path_is_valid) {
                spdlog::error("The provided airbag is not valid, cannot load the texture"); 
                throw DirectInvalidArgument();
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
                if(load_airbag)
                    image_texture = load_direct_image(renderer_handle, std::string(image_airbag).c_str());
                else
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
                spdlog::warn("Loading the font airbag");
                
                // Load the font airbag
                text_texture = SDL::DirectTextureFactory::load_texture(renderer, "", "data/assets/undefined.png", true);
            } else {
                // Convert the texture surface to the font texture
                text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

                if(text_texture == nullptr) {
                  // Texture airbag loading failed, so rasing the exception
                  spdlog::error("{}", SDL_GetError());
                  throw SDL::DirectSystemException();
                }

                // Free the unnecesarry data
                SDL_FreeSurface(text_surface);
            }

            return text_texture;
        }
    };
}
