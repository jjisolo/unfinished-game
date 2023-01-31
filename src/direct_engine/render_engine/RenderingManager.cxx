#include "RenderingManager.h"

[[maybe_unused]] void SDL::RenderingManager::bind_render_scene(const SDL::RenderSceneTag& render_scene_tag, SDL::RenderScene render_scene)
{
    // If we are rewriting the render scene, it means we throw the exception
    // because we can't disable it(due to the class structure).
    if(m_render_scene_container.contains(render_scene_tag))
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "\t--- Binding of the existing render scene: %s", render_scene_tag.c_str());

    m_render_scene_container.emplace(render_scene_tag, std::move(render_scene));
}

[[maybe_unused]] void SDL::RenderingManager::set_active_render_scene(const SDL::RenderSceneTag &render_scene_tag)
{
    // If we are rewriting the render scene, it means we throw the exception
    // because we can't disable it(due to the class structure).
    if(!m_render_scene_container.contains(render_scene_tag))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\t--- The render scene width a tag %s does not contained in the map", render_scene_tag.c_str());
        throw SDL::DirectInvalidArgument();
    }

    m_active_render_scene = render_scene_tag;
}

void SDL::RenderingManager::render()
{
    // Get the all enabled render groups
    try
    {
        auto& active_render_scene        = m_render_scene_container.at(m_active_render_scene);
        auto  enabled_render_groups_list = active_render_scene.get_enabled_render_groups();

        // Iterate through each enabled render group
        for(auto& enabled_render_group_id: enabled_render_groups_list)
        {
            auto  enabled_render_group_vector_index = active_render_scene.get_render_group_by_id(enabled_render_group_id);
            auto& enabled_render_group              = active_render_scene.m_render_groups.at(enabled_render_group_vector_index);

            for(auto& enabled_render_object: enabled_render_group)
            {
                try
                {
                    std::visit([&active_render_scene](auto&& render_object_backend) {
                        using RenderObjectType = std::decay_t<decltype(render_object_backend)>;
                        // Iterate through each variant type, and do the job for each type.
                        if constexpr(std::is_same_v<RenderObjectType, SDL::DirectTextureContainer>) {
                            render_object_backend.render(active_render_scene.m_binded_renderer_handle);
                        } else {
                            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\t--- Non-exhaustive visitor in [SDL::RenderingManager::render]");
                        }
                    }, enabled_render_object);
                }
                catch(const std::bad_variant_access& exception)
                {
                    // The render_object is somehow valueless_by_exception, we are
                    // just logging the error and then to the rest of the job
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "The render_object is valueless_by_exception!");
                }
            }
        }

        SDL_RenderPresent(active_render_scene.m_binded_renderer_handle);
    }
    catch(const std::out_of_range& exception)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "\t--- Can't get access to the render scene with Tag %s", m_active_render_scene.c_str());
        return;
    }
}
