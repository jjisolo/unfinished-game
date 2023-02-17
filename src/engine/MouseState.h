#pragma once

#include <SDL.h>
#include <array>
#include <utility>

#include <RenderingExceptions.h>

namespace SDL::priv
{
    /*! Internal constant for the mouse left button index in the MouseState class */
    constexpr const int MouseButtonLeft   = 0;
    /*! Internal constant for the mouse right button index in the MouseState class */
    constexpr const int MouseButtonRight  = 1;
    /*! Internal constant for the mouse middle button index in the MouseState class */
    constexpr const int MouseButtonMiddle = 2;

    /*!
     * \brief Represents the current state of the mouse
     *
     * As an internal part of the EngineInterface class. Holds all
     * information about current mouse state: pressed buttons, mouse
     * position and motion etc.
     *
     * \sa EngineInterface
     */
    class MouseState
    {
        using MouseButtonState  = bool;
        using MouseMotionState  = std::pair<int, int>;
        using MouseScrollState  = std::pair<int, int>;
        using MouseButtonStates = std::array<MouseButtonState, 3>;

    public:
        MouseState()  = default;
        ~MouseState() = default;

    public:
        /*! \brief Update the mouse state */
        [[maybe_unused]] void update(SDL_Event& event);

        /*! \brief Get the pressed state of the any mouse button */
        [[maybe_unused]] [[nodiscard]] inline MouseButtonState get_button_press_state(int index) const
        {
            if(index <= MouseButtonMiddle && index >= 0)
                return m_button_states[index];
            else
                throw SDL::DirectInvalidArgument();
        }

        /*! \brief Get the motion state of the mouse(its coordinates) */
        [[maybe_unused]] [[nodiscard]] inline MouseMotionState get_motion_state()     const { return m_motion_state; }

        /*! \brief Get the scroll state of the mouse */
        [[maybe_unused]] [[nodiscard]] inline MouseMotionState get_scrolling_state()  const { return m_scroll_state; }

    private:
        MouseButtonStates m_button_states;
        MouseMotionState  m_motion_state;
        MouseScrollState  m_scroll_state;
    };
}
