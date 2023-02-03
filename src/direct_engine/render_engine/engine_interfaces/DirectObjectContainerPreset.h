#pragma once

#include "RenderingEngine.h"

#include <utility>

namespace SDL
{
    class DirectTextureFactory;

    using DirectRendererHandle = SDL_Renderer*;
    using DirectObjectName     = std::string;

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
        [[maybe_unused]] DirectObjectContainerPreset(
                SDL::DirectObjectName  direct_object_name,
                SDL::DirectObjectState direct_object_state)
                :
                m_direct_object_state(direct_object_state),
                m_direct_object_name(std::move(direct_object_name))
        {
        }

        /*!
        * \brief Render the object on the screen
        */
        [[maybe_unused]] virtual void render(SDL::DirectRendererHandle direct_renderer_handle) const = 0;

        /*!
         * \brief Enable the object
         */
        [[maybe_unused]] virtual void enable(SDL::DirectRendererHandle direct_renderer_handle) = 0;

        /*!
        * \brief Disable the object
         */
        [[maybe_unused]] virtual void disable() = 0;

        /*!
        * \brief Get the object state
        */
        [[maybe_unused]] [[nodiscard]] inline SDL::DirectObjectState get_state() const
        {
            return m_direct_object_state;
        }

        /*!
        * \brief Get the object name
        */
        [[maybe_unused]] [[nodiscard]] inline SDL::DirectObjectName get_name() const
        {
            return m_direct_object_name;
        }

    protected:
        SDL::DirectObjectState m_direct_object_state;
        SDL::DirectObjectName  m_direct_object_name;
    };

}
