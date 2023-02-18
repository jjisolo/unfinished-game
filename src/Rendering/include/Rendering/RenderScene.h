#pragma once

#include <Rendering/RenderingEngine.h>
#include <Rendering/DirectTextureContainer.h>

#include <utility>

namespace SDL
{
    using DirectFontHandle         = TTF_Font*;
    using DirectFontName           = std::string;
    using DirectFontTextContainer  = const std::string_view&;

    class DirectFontContainer
    {
    public:
        explicit DirectFontContainer(
                SDL::DirectFontRelativePath   font_path,
                SDL::DirectFontDisplaySize    font_size    = 12,
                SDL::DirectFontDisplayVariant font_variant = SDL::DirectFontDisplayVariant :: Normal,
                SDL::DirectFontRenderKind     font_kind    = SDL::DirectFontRenderKind     :: BlendedWrapped):
            path(std::move(font_path)), size(font_size), variant(font_variant), kind(font_kind), _data(nullptr)
        {
        }

        SDL::DirectFontRelativePath   path;
        SDL::DirectFontDisplaySize    size;
        SDL::DirectFontDisplayVariant variant;
        SDL::DirectFontRenderKind     kind;
        SDL::DirectFontHandle         _data;
    };

    using RenderObject         = std::variant<SDL::DirectTextureContainer, SDL_Texture*>;

	using RenderGroup          = std::vector<SDL::RenderObject>;
	using RenderGroupContainer = std::vector<SDL::RenderGroup>;
	using RenderGroupName      = std::string;
	using RenderGroupID        = int;
    using RenderGroupIDList    = std::list<SDL::RenderGroupID>;

	using RenderGroupIDContainer    = std::vector       <SDL::RenderGroupID>;
	using RenderGroupNameContainer  = std::unordered_map<std::string, SDL::RenderGroupID>;

    using RenderSceneFontContainer  = std::unordered_map<SDL::DirectFontName, struct SDL::DirectFontContainer>;

    using SharedTexturePosition     = std::pair<std::size_t, std::size_t>;

    /*!
     * Render scene is a container for a different render groups(which are build
     * from the groups of render objects).
     *
     * Render group management is handled using render group ID's and names
     * (which can be aliased to some ID).
     *
     * The common practise is to split the render scene in different render groups
     * and then enable or disable some of them. For example let's take an abstract
     * render scene named `main menu`, it would be built from render group called
     * `menu_main`, `submenu_settings` etc. Of course on the start of the menu scene
     * we dont need to render the `submenu_settings` group.
     *
     * That is why the render scene is so usefully when it comes to the actual development
     * of a dynamic render scene.
     */
    class RenderScene
    {
        friend class RenderingManager;
    public:
        /*!
        * \brief Construct RenderScene class
        *
        * \param direct_renderer_handle a handle to an legacy SDL2 renderer
        *
        * \sa SDL::EngineInterface::get_renderer_handle()
        */
        explicit RenderScene(SDL::DirectRendererHandle direct_renderer_handle): m_binded_renderer_handle(direct_renderer_handle)
        {
        }

		/*!
		* \brief Get distinct(unique) render group ID
		* 
		* It is a very slow function(lets hope that is for now)
		*/
		RenderGroupID get_distinct_render_group_id();

		/*!
		* \brief Alias some render group ID to some distinct string.
		*
		* \param render_group_id id of the render group to be aliased
		* \param aliased_name name to be aliased  
		*/
        [[maybe_unused]] void alias_render_group(SDL::RenderGroupID render_group_id, const RenderGroupName& aliased_name);

		/*!
		* \brief Push render object to the render group using aliased name
		*
		* \param render_group aliased name of the render group
		* \param render_object render object that is moved to the render group
		*/
        [[maybe_unused]] void push_to_render_group(const SDL::RenderGroupName& render_group, SDL::RenderObject render_object);

		/*!
		* \brief Push render object to the render group using render group ID
		*
		* \param render_group ID of the render group
		* \param render_object render object that is moved to the render group
		*/
        [[maybe_unused]] void push_to_render_group(SDL::RenderGroupID render_group, SDL::RenderObject render_object);

        /*!
		* \brief Push render text(as texture) to the render group using render group ID
		*
		* \param render_group ID of the render group
		* \param text         The text that is going to be rendered
		*/
        [[maybe_unused]] void push_text_to_render_group(
                SDL::RenderGroupID           render_group,
                SDL::DirectFontName          font_name,
                SDL::DirectFontTextContainer text         = "Sample text",
                SDL::DirectColor             color_fg     = {255, 255, 255, 255},
                SDL::DirectColor             color_bg     = {0,   0,   0,   255},
                SDL::SharedTexturePosition   destination  = {0,   0});

        /*!
		* \brief Enable render group
		*
        * Make the render group able to be rendered by loading all objects
        * that it contains
        *
		* \param render_group ID of the render group
		*/
        [[maybe_unused]] void enable_render_group(SDL::RenderGroupID render_group);

        /*!
		* \brief Disable render group
		*
        * Make the render group unable to be rendered by unloading all objects
        * that it contains
        *
		* \param render_group ID of the render group
		*/
        [[maybe_unused]] void disable_render_group(SDL::RenderGroupID render_group_id);

        /*!
        * \brief Disable render group
        *
        * Make the render group able to be rendered by unloading all objects
        * that it contains
        *
        * \param render_group Aliased name of the render group
        *
        * \sa RenderScene::alias_render_group()
        */
        [[maybe_unused]] void enable_render_group(const SDL::RenderGroupName& render_group);

        /*!
        * \brief Disable render group
        *
        * Make the render group unable to be rendered by unloading all objects
        * that it contains
        *
        * \param render_group Aliased name of the render group
        *
        * \sa RenderScene::alias_render_group()
        */
        [[maybe_unused]] void disable_render_group(const SDL::RenderGroupName& render_group);

        /*!
         * \brief Get all enabled render groups
         *
         * \sa RenderGroupIDList
         */
        [[maybe_unused]] inline SDL::RenderGroupIDList get_enabled_render_groups() const
        {
            return m_enabled_render_groups;
        }

        /*!
         * \brief Get all enabled render groups
         *
         * \sa RenderGroupIDList
         */
        [[maybe_unused]] inline void add_supported_font(const std::string& font_tag, SDL::DirectFontContainer font)
        {
            // Validate the given structure
            assert_font_container_invariants(font);

            // Load the font
            load_font_from_container(font);

            if(!m_fonts_container.contains(font_tag))
                m_fonts_container.insert({font_tag, font});
            else
                spdlog::warn("Attempted to add font with tag that already contains in the render scene");
        }

        /*!
         * \brief Get all enabled render groups
         *
         * \sa RenderGroupIDList
         */
        [[maybe_unused]] inline void remove_supported_font(const std::string& font_tag)
        {
            if(!m_fonts_container.contains(font_tag))
                spdlog::warn("Attempted to remove font with tag that do not contains in the render scene");
            else
                m_fonts_container.erase(font_tag);
        }

    private:
		std::ptrdiff_t get_render_group_by_id(RenderGroupID render_group);

        /*!
         * \brief Load the font using the information from the given container
         *
         * \sa assert_font_container_invariants()
         * \sa SDL::DirectFontContainer
         */
        [[maybe_unused]] static void load_font_from_container(SDL::DirectFontContainer& font)
        {
            // Load the font using the SDL2_ttf library
            font._data = TTF_OpenFont(std::string(font.path).c_str(), static_cast<int>(font.size));

            // Check if the font loaded correctly
            if(font._data == nullptr) {
                spdlog::error("Unable to load font {}", std::string(font.path).c_str());
                spdlog::error("{}", TTF_GetError());
                throw SDL::DirectTrueTypeSystemException();
            }

            int font_style{TTF_STYLE_NORMAL};
            switch (font.variant)
            {
                case SDL::DirectFontDisplayVariant::Normal:              font_style = TTF_STYLE_NORMAL;                         break;
                case SDL::DirectFontDisplayVariant::Bold:                font_style = TTF_STYLE_BOLD;                           break;
                case SDL::DirectFontDisplayVariant::BoldItalic:          font_style = TTF_STYLE_BOLD|TTF_STYLE_ITALIC;          break;
                case SDL::DirectFontDisplayVariant::Italic:              font_style = TTF_STYLE_ITALIC;                         break;
                case SDL::DirectFontDisplayVariant::Underline:           font_style = TTF_STYLE_UNDERLINE;                      break;
                case SDL::DirectFontDisplayVariant::UnderlineItalic:     font_style = TTF_STYLE_UNDERLINE|TTF_STYLE_ITALIC;     break;
                case SDL::DirectFontDisplayVariant::StrikeThrough:       font_style = TTF_STYLE_STRIKETHROUGH;                  break;
                case SDL::DirectFontDisplayVariant::StrikeThroughItalic: font_style = TTF_STYLE_STRIKETHROUGH|TTF_STYLE_ITALIC; break;
                default: font_style = TTF_STYLE_NORMAL; break;
            }

            TTF_SetFontStyle  (font._data, font_style);
            TTF_SetFontHinting(font._data, TTF_HINTING_LIGHT_SUBPIXEL);
        }

        /*!
         * \brief Assert the font structure invariants
         *
         * Validate variables that are present in the DirectFontContainer structure
         *
         * \sa SDL::DirectFontContainer
         */
        [[maybe_unused]] static void assert_font_container_invariants(SDL::DirectFontContainer& font)
        {
            if(font.path.empty()) {
                spdlog::error("Attempted to load the font with invalid font path");
                throw DirectInvalidArgument();
            }

            if(font.size <= 0) {
                spdlog::error("Attempted to load the font with invalid font size");
                throw DirectInvalidArgument();
            }

            if(font._data != nullptr) {
                spdlog::warn("Attempted to load the font that is already in the memory");
                spdlog::warn("Cleaning up the previous font data...");
                TTF_CloseFont(font._data);
            }
        }

	private:
        SDL::DirectRendererHandle      m_binded_renderer_handle;
		SDL::RenderGroupContainer      m_render_groups;
        SDL::RenderGroupIDList         m_enabled_render_groups;
        SDL::RenderGroupIDContainer    m_render_group_id_to_internal_id;
        SDL::RenderGroupNameContainer  m_render_group_names;

        SDL::RenderSceneFontContainer  m_fonts_container;
    };
}