#pragma once

#include <RenderingEngine.h>
#include <RenderObject.h>

namespace SDL
{
	using RenderGroupIndicies  = std::vector<std::uint8_t>;
	using RenderGroup          = std::vector<SDL::RenderObject>;
	using RenderGroupContainer = std::vector<SDL::RenderGroup>;
	using RenderGroupName      = std::string;
	using RenderGroupID        = int;

	using RenderGroupIDContainer    = std::vector       <SDL::RenderGroupID>;
	using RenderGroupIDToInternalID = std::vector       <SDL::RenderGroupID>;
	using RenderGroupNameContainer  = std::unordered_map<std::string, SDL::RenderGroupID>;

	class RenderScene
	{
	public:
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
		void alias_render_group(const RenderGroupID render_group_id, RenderGroupName aliased_name);

		/*!
		* \brief Push render object to the render group using aliased name
		*
		* \param render_group aliased name of the render group
		* \param render_object render object that is moved to the render group
		*/
		void push_to_render_group(const RenderGroupName& render_group, RenderObject render_object);

		/*!
		* \brief Push render object to the render group using render group ID
		*
		* \param render_group ID of the render group
		* \param render_object render object that is moved to the render group
		*/
		void push_to_render_group(const RenderGroupID render_group, RenderObject render_object);

		void enable_render_group(const RenderGroupID render_group_id);

		void disable_render_group(const RenderGroupID render_group_id);

	private:

		std::ptrdiff_t get_render_group_by_id(const RenderGroupID render_group);

	private:
		RenderGroupContainer      m_render_groups;
		RenderGroupIDContainer    m_active_render_groups;
		RenderGroupIDContainer    m_render_group_id_to_internal_id;
		RenderGroupNameContainer  m_render_group_names;
	};
}