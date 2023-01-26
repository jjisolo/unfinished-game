#pragma once

#include <RenderingEngine.h>

namespace SDL
{
    using DirectRendererHandle = SDL_Renderer*;

    enum class DirectObjectState : std::uint8_t
    {
        Enabled,
        Disabled
    };
}

namespace SDL::priv
{
    class DirectObjectContainerPreset
    {
    public:
        [[maybe_unused]] explicit DirectObjectContainerPreset(SDL::DirectObjectState direct_object_state):
            m_direct_object_state(direct_object_state)
        {

        }

        /*!
        * \brief Render the object on the screen
        */
        [[maybe_unused]] virtual void render(SDL::DirectRendererHandle direct_renderer_handle)=0;

        /*!
         * \brief Enable the object
         */
        [[maybe_unused]] virtual void enable(SDL::DirectRendererHandle direct_renderer_handle)=0;

        /*!
        * \brief Disable the object
         */
        [[maybe_unused]] virtual void disable()=0;

        /*!
        * \brief Get the object state
        */
        [[maybe_unused]] [[nodiscard]] inline SDL::DirectObjectState get_state() const
        {
            return m_direct_object_state;
        }

    protected:
        SDL::DirectObjectState m_direct_object_state;
    };

}
