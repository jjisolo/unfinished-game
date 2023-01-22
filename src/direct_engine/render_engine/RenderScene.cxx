#include <RenderScene.h>

std::ptrdiff_t SDL::RenderScene::get_render_group_by_id(const SDL::RenderGroupID render_group)
{
	// Find the required ID in the global id->user_id table
	auto vector_iterator = VectorFind(m_render_group_id_to_internal_id, render_group);

	// If we found something calculate the from the start of the vector
	// to the element(read: find its index) and return it
	if (vector_iterator != m_render_group_id_to_internal_id.end())
		return std::distance(m_render_group_id_to_internal_id.begin(), vector_iterator);

	// Otherwise return throw some abstract error
	throw std::invalid_argument("Specified ID does not exists in the table");
}


void SDL::RenderScene::push_to_render_group(const SDL::RenderGroupName& render_group, SDL::RenderObject render_object)
{
	// Check if this name exists in the name container, if it
	// does not create it and asign to the current id.
	if (!m_render_group_names.contains(render_group))
	{
		alias_render_group(get_distinct_render_group_id(), render_group);
	}

	// Otherwise string->id is exists, so we just pull it from
	// the map
	push_to_render_group(m_render_group_names[render_group], std::move(render_object));
}

void SDL::RenderScene::push_to_render_group(const SDL::RenderGroupID render_group, SDL::RenderObject render_object)
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::RenderScene::push_to_render_group [overloaded]");

	// The index of the render group associated with the render_group ID
	std::ptrdiff_t render_group_index{0};

	try
	{
		// Find out the index of the render group by its ID
		render_group_index = get_render_group_by_id(render_group);
	}
	catch (const std::invalid_argument& exception)
	{
		// As soon as the specified ID is not in the table, the new
		// render group should be created
		m_render_group_id_to_internal_id.push_back(render_group);
		m_render_groups                 .push_back(SDL::RenderGroup());

		render_group_index = m_render_group_id_to_internal_id.size() - 1;
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- The new render group container has been created");
	}

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Object %s is added to the render group with ID of %d", render_object.title.c_str(), render_group);

	// Move the element to the render group
	m_render_groups.at(render_group_index).push_back(std::move(render_object));
}

void SDL::RenderScene::alias_render_group(const SDL::RenderGroupID render_group_id, SDL::RenderGroupName aliased_name)
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::RenderScene::alias_render_group [%p]", &SDL::RenderScene::alias_render_group);

	// If the render group with the same name is already exists
	if (m_render_group_names.contains(aliased_name))
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\t--- The name %s is already aliased", aliased_name.c_str());

		// Throw the exception
		throw std::invalid_argument("Specified name does already exists in the table");
	}

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- The name %s was aliased to the ID of %d", aliased_name.c_str(), render_group_id);

	// Otherwise do the alias
	m_render_group_names[aliased_name] = render_group_id;
}

SDL::RenderGroupID SDL::RenderScene::get_distinct_render_group_id()
{
	SDL::RenderGroupID render_group_id_iterator{0};

	while (!VectorFindBinary(m_render_group_id_to_internal_id, render_group_id_iterator))
		render_group_id_iterator++;

	return render_group_id_iterator;
}


void SDL::RenderScene::enable_render_group(const SDL::RenderGroupID render_group_id)
{
	auto render_group_valid_index = get_render_group_by_id(render_group_id);

	for (auto& render_object : m_render_groups.at(render_group_valid_index))
	{
		if (render_object.kind != SDL::RenderObjectKind::Enabled)
		{
			switch (render_object.type)
			{
				case SDL::RenderObjectType::DirectTexture:
				{
					if (render_object.kind != SDL::RenderObjectKind::Enabled)
					{
						auto render_object_backend = std::get<SDL_Texture*>(render_object.backend);

						
					}
					break;
				}
			}
		}
	}
}

void SDL::RenderScene::disable_render_group(const SDL::RenderGroupID render_group_id)
{

}