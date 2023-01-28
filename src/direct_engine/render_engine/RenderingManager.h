#pragma once

#include <RenderingExceptions.h>
#include <RenderingEngine.h>
#include <RenderScene.h>

namespace SDL
{
    using RenderSceneContainer = std::unordered_map<std::string, SDL::RenderScene>;
    using RenderSceneTag       = std::string;

	class RenderingManager
	{
    public:
        [[maybe_unused]] void bind_render_scene      (const SDL::RenderSceneTag& render_scene_tag, SDL::RenderScene render_scene);

        [[maybe_unused]] void set_active_render_scene(const SDL::RenderSceneTag& render_scene_tag);

        [[maybe_unused]] void render                 ();

    private:
        SDL::RenderSceneContainer m_render_scene_container;
        SDL::RenderSceneTag       m_active_render_scene;
	};
}