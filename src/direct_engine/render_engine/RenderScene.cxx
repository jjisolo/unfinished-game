#include <DirectTextureFactory.h>
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

		render_group_index = static_cast<std::ptrdiff_t>(m_render_group_id_to_internal_id.size() - 1);
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

		// Throw the invalid_argument exception as the specified name already exists in the table
		throw SDL::DirectInvalidArgument();
	}

	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- The name %s was aliased to the ID of %d", aliased_name.c_str(), render_group_id);

	// Otherwise do the alias
	m_render_group_names[aliased_name] = render_group_id;
}

void SDL::RenderScene::push_text_to_render_group(
    SDL::RenderGroupID           render_group,
    SDL::DirectFontName          font_name,
    SDL::DirectFontTextContainer text,
    SDL::DirectColor             color_fg,
    SDL::DirectColor             color_bg,
    SDL::SharedTextureRect       source,
    SDL::SharedTextureRect       destination)
{
    if(!m_fonts_container.contains(font_name)) {
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- Font %s does not supported", font_name.c_str());
        throw SDL::DirectInvalidArgument();
    }

    // The index of the render group associated with the render_group ID
    std::ptrdiff_t render_group_index;

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

        render_group_index = static_cast<std::ptrdiff_t>(m_render_group_id_to_internal_id.size() - 1);
        SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "\t--- The new render group container has been created");
    }

    m_render_groups.at(render_group_index).emplace_back(
        SDL::DirectTextureContainer(
            SDL::DirectTextureFactory::load_font(
                m_binded_renderer_handle,
                m_fonts_container.at(font_name)._data,
                m_fonts_container.at(font_name).kind,
                std::string(text), color_fg, color_bg
            ),
            font_name, source, destination
        )
    );
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
        try
        {
            std::visit([this, &render_group_id](auto&& render_object_backend) {
                using RenderObjectType = std::decay_t<decltype(render_object_backend)>;
                // Iterate through each variant type, and do the job for each type.
                if constexpr(std::is_same_v<RenderObjectType, SDL::DirectTextureContainer>) {
                    m_enabled_render_groups.push_back(static_cast<int>(render_group_id));
                    render_object_backend  .enable(m_binded_renderer_handle);
                } else {
                    // The visitor did not find any appropriate type, for now it could be either because
                    // the project is under development and this variant can change frequently or because
                    // the user passed the wrong type to the variant. In all cases the abort exception
                    // should now be thrown as it is not an critical issue
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\t--- Non-exhaustive visitor in [SDL::RenderingManager::enable_render_group]");
                }
            }, render_object);
        }
        catch(const std::bad_variant_access& exception)
        {
            // The render_object is somehow valueless_by_exception, we are
            // just logging the error and then to the rest of the job
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "The render_object is valueless_by_exception!");
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
        try
        {
            std::visit([this, &render_group_id](auto&& render_object_backend) {
                using RenderObjectType = std::decay_t<decltype(render_object_backend)>;
                // Iterate through each variant type, and do the job for each type.
                if constexpr(std::is_same_v<RenderObjectType, SDL::DirectTextureContainer>) {
                    m_enabled_render_groups.remove(static_cast<int>(render_group_id));
                    render_object_backend  .disable();
                }
            }, render_object);
        }
        catch(const std::bad_variant_access& exception)
        {
            // The render_object is somehow valueless_by_exception, we are
            // just logging the error and then to the rest of the job
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "The render_object is valueless_by_exception!");
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
