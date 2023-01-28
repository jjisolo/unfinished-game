#include <RenderScene.h>

std::ptrdiff_t SDL::RenderScene::get_render_group_by_id(const SDL::RenderGroupID render_group)
{
	// Find the required ID in the global id->user_id table
	auto vector_iterator = VectorFind(m_render_group_id_to_internal_id, render_group);

	// If we found something calculate from the start of the vector
	// to the element(read: find its index) and return it
	if (vector_iterator != m_render_group_id_to_internal_id.end())
		return std::distance(m_render_group_id_to_internal_id.begin(), vector_iterator);

	// Otherwise return throw some abstract error
	throw std::invalid_argument("Specified ID does not exists in the table");
}


[[maybe_unused]] void SDL::RenderScene::push_to_render_group(const SDL::RenderGroupName& render_group, SDL::RenderObject render_object)
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
		m_render_groups                 .emplace_back();

		render_group_index = m_render_group_id_to_internal_id.size() - 1;
		SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- The new render group container has been created");
	}

	// Move the element to the render group
	m_render_groups.at(render_group_index).push_back(std::move(render_object));
}

void SDL::RenderScene::alias_render_group(const SDL::RenderGroupID render_group_id, const SDL::RenderGroupName& aliased_name)
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "Entering SDL::RenderScene::alias_render_group [%p]", this);

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


[[maybe_unused]] void SDL::RenderScene::enable_render_group(const SDL::RenderGroupID render_group_id)
{
	auto render_group_valid_index = get_render_group_by_id(render_group_id);

    // Iterate through each render object at the render group, which id is provided by
    // the user
	for (auto& render_object : m_render_groups.at(render_group_valid_index))
	{
        // As soon as each render object backend contained in the union render_object
        // is a standardized object with enable/disable/render function inside of it
        // we are free to just check if the container is empty, and otherwise just call
        // the enable function
        if(render_object.index() != std::variant_npos)
        {
            switch(render_object.index())
            {
                // The variant holds the SDL::DirectTextureContainer class
                case SDL::priv::RenderObjectVariantIndex_DirectTextureContainer:
                {
                    auto& render_object_backend = std::get<SDL::DirectTextureContainer>(render_object);
                    render_object_backend  .enable(m_binded_renderer_handle, m_binded_texture_factory);
                    m_enabled_render_groups.push_back(static_cast<int>(render_group_id));
                    break;
                }
            }
        }
	}
}

[[maybe_unused]] void SDL::RenderScene::enable_render_group(const SDL::RenderGroupName& render_group)
{
    // Check if this name exists in the name container, if it
    // does not create it and asign to the current id.
    if (!m_render_group_names.contains(render_group))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\t--- The name %s is not aliased within any known render group", render_group.c_str());
        throw SDL::DirectInvalidArgument();
    }

    // Otherwise string->id is exists, so we just pull it from
    // the map
    enable_render_group(m_render_group_names[render_group]);
}

[[maybe_unused]] void SDL::RenderScene::disable_render_group(const SDL::RenderGroupID render_group_id)
{
    auto render_group_valid_index = get_render_group_by_id(render_group_id);

    // Iterate through each render object at the render group, which id is provided by
    // the user
    for (auto& render_object : m_render_groups.at(render_group_valid_index))
    {
        // As soon as each render object backend contained in the union render_object
        // is a standardized object with enable/disable/render function inside of it
        // we are free to just check if the container is empty, and otherwise just call
        // the enable function
        if(render_object.index() != std::variant_npos)
        {
            switch(render_object.index())
            {
                // The variant holds the SDL::DirectTextureContainer class
                case SDL::priv::RenderObjectVariantIndex_DirectTextureContainer:
                {
                    auto render_object_backend = std::get<SDL::DirectTextureContainer>(render_object);
                    render_object_backend  .disable();
                    m_enabled_render_groups.remove(static_cast<int>(render_group_id));
                    break;
                }
            }
        }
    }
}

[[maybe_unused]] void SDL::RenderScene::disable_render_group(const SDL::RenderGroupName& render_group)
{
    // Check if this name exists in the name container, if it
    // does not create it and assign to the current id.
    if (!m_render_group_names.contains(render_group))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\t--- The name %s is not aliased within any known render group", render_group.c_str());
        throw SDL::DirectInvalidArgument();
    }

    // Otherwise string->id is exists, so we just pull it from
    // the map
    disable_render_group(m_render_group_names[render_group]);
}