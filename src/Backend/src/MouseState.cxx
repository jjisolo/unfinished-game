#include <Backend/MouseState.h>

[[maybe_unused]] void SDL::priv::MouseState::update(SDL_Event& event) {
    // Poll for currently pending events.
    SDL_PollEvent(&event);

    switch (event.type) {
        // Parse the mouse button press event
        case SDL_MOUSEBUTTONDOWN:
            switch(event.button.button) {
                case SDL_BUTTON_LEFT:
                    m_button_states[MouseButtonLeft]   = true;
                    break;
                case SDL_BUTTON_RIGHT:
                    m_button_states[MouseButtonRight]  = true;
                    break;
                case SDL_BUTTON_MIDDLE:
                    m_button_states[MouseButtonMiddle] = true;
                    break;
            }
            break;
        // Parse the mouse button release event
        case SDL_MOUSEBUTTONUP:
            switch(event.button.button) {
                case SDL_BUTTON_LEFT:
                    m_button_states[MouseButtonLeft]   = false;
                    break;
                case SDL_BUTTON_RIGHT:
                    m_button_states[MouseButtonRight]  = false;
                    break;
                case SDL_BUTTON_MIDDLE:
                    m_button_states[MouseButtonMiddle] = false;
                    break;
            }
            break;
        // Parse the mouse move event
        case SDL_MOUSEMOTION:
            m_motion_state.first  = event.motion.x;
            m_motion_state.second = event.motion.y;
            break;
        // Parse the mousewheel toggle event
        case SDL_MOUSEWHEEL:
            m_scroll_state.first  = event.wheel.x;
            m_scroll_state.second = event.wheel.y;
            break;
    }
}
