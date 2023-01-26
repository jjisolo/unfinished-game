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

    using RenderObject         = std::variant<SDL::DirectTextureContainer, SDL_Texture*>;

	using RenderGroup          = std::vector<SDL::RenderObject>;
	using RenderGroupContainer = std::vector<SDL::RenderGroup>;
	using RenderGroupName      = std::string;
	using RenderGroupID        = int;

	using RenderGroupIDContainer    = std::vector       <SDL::RenderGroupID>;
	using RenderGroupNameContainer  = std::unordered_map<std::string, SDL::RenderGroupID>;

    /*!
     *
     */
    class RenderScene
    {
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
		void alias_render_group(SDL::RenderGroupID render_group_id, const RenderGroupName& aliased_name);

		/*!
		* \brief Push render object to the render group using aliased name
		*
		* \param render_group aliased name of the render group
		* \param render_object render object that is moved to the render group
		*/
		void push_to_render_group(const SDL::RenderGroupName& render_group, SDL::RenderObject render_object);

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

	private:
		std::ptrdiff_t get_render_group_by_id(RenderGroupID render_group);

	private:
        std::shared_ptr<SDL::DirectTextureFactory> m_binded_texture_factory;

        SDL::DirectRendererHandle      m_binded_renderer_handle;
		SDL::RenderGroupContainer      m_render_groups;
        SDL::RenderGroupIDContainer    m_active_render_groups;
        SDL::RenderGroupIDContainer    m_render_group_id_to_internal_id;
        SDL::RenderGroupNameContainer  m_render_group_names;
	};
}