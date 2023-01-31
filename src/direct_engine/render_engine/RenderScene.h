#pragma once

#include <RenderingEngine.h>
#include <DirectTextureContainer.h>

namespace SDL
{
    namespace priv
    {
        // RenderObject variant index(std::variant::index) to the data structure
        // name, absolutely redundant just the code sugar
        static constexpr const std::uint8_t RenderObjectVariantIndex_DirectTextureContainer = 0u;
        // ...
    }

    struct DirectFontContainer
    {
        SDL::DirectFontRelativePath   path;
        SDL::DirectFontDisplaySize    size;
        SDL::DirectFontDisplayVariant variant;
    };

    using RenderObject         = std::variant<SDL::DirectTextureContainer, SDL_Texture*>;

	using RenderGroup          = std::vector<SDL::RenderObject>;
	using RenderGroupContainer = std::vector<SDL::RenderGroup>;
	using RenderGroupName      = std::string;
	using RenderGroupID        = int;
    using RenderGroupIDList    = std::list<SDL::RenderGroupID>;

	using RenderGroupIDContainer    = std::vector       <SDL::RenderGroupID>;
	using RenderGroupNameContainer  = std::unordered_map<std::string, SDL::RenderGroupID>;

    using RenderSceneFontContainer  = std::unordered_map<std::string, struct SDL::DirectFontContainer>;


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
        * \param direct_texture_factory a shared pointer with an initialized
        * texture factory
        *
        * \sa SDL::DirectTextureFactory
        * \sa SDL::EngineInterface::get_renderer_handle()
        *
        */
        explicit RenderScene(
                SDL::DirectRendererHandle                  direct_renderer_handle,
                std::shared_ptr<SDL::DirectTextureFactory> direct_texture_factory

        ):
                m_binded_renderer_handle(direct_renderer_handle),
                m_binded_texture_factory(std::move(direct_texture_factory))
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
        [[maybe_unused]] void disable_render_group(SDL::RenderGroupID render_group);

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
            if(!m_fonts_container.contains(font_tag))
                m_fonts_container[font_tag] = std::move(font);
            else
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\t--- Attempted to add font with tag that already contains in the render scene");
        }

        /*!
         * \brief Get all enabled render groups
         *
         * \sa RenderGroupIDList
         */
        [[maybe_unused]] inline void remove_supported_font(const std::string& font_tag)
        {
            if(!m_fonts_container.contains(font_tag))
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\t--- Attempted to remove font with tag that do not contains in the render scene");
            else
                m_fonts_container.erase(font_tag);
        }


    private:
		std::ptrdiff_t get_render_group_by_id(RenderGroupID render_group);

	private:
        std::shared_ptr<SDL::DirectTextureFactory> m_binded_texture_factory;

        SDL::DirectRendererHandle      m_binded_renderer_handle;
		SDL::RenderGroupContainer      m_render_groups;
        SDL::RenderGroupIDList         m_enabled_render_groups;
        SDL::RenderGroupIDContainer    m_render_group_id_to_internal_id;
        SDL::RenderGroupNameContainer  m_render_group_names;

        SDL::RenderSceneFontContainer  m_fonts_container;
	};
}