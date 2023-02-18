#include <Backend/KeyboardState.h>

[[maybe_unused]] void SDL::priv::KeyboardState::update(SDL_Event& event)
{
    // Poll for currently pending events
    SDL_PollEvent(&event);

    // Value to assign to the button, depends on whether the key is down
    // or it is released
    auto assign_value{false};

    if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
        assign_value = true;
    else if (event.type == SDL_KEYUP)
        assign_value = false;

    // If the key is down, and it is not held(we don't want to produce
    // unnecessary keyboard events)
    if((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && event.key.repeat == 0) {
        switch(event.key.keysym.sym) {
            using namespace std::literals;
            // ~ -> PageDown keyboard row
            case SDLK_0: m_current_key_states["0"sv] = assign_value; break;
            case SDLK_1: m_current_key_states["1"sv] = assign_value; break;
            case SDLK_2: m_current_key_states["2"sv] = assign_value; break;
            case SDLK_3: m_current_key_states["3"sv] = assign_value; break;
            case SDLK_4: m_current_key_states["4"sv] = assign_value; break;
            case SDLK_5: m_current_key_states["5"sv] = assign_value; break;
            case SDLK_6: m_current_key_states["6"sv] = assign_value; break;
            case SDLK_7: m_current_key_states["7"sv] = assign_value; break;
            case SDLK_8: m_current_key_states["8"sv] = assign_value; break;
            case SDLK_9: m_current_key_states["9"sv] = assign_value; break;
            case SDLK_BACKSPACE: m_current_key_states["backspace"sv] = assign_value; break;
            case SDLK_BACKQUOTE: m_current_key_states["backquote"sv] = assign_value; break;
            case SDLK_INSERT:    m_current_key_states["insert"sv]    = assign_value; break;
            case SDLK_HOME:      m_current_key_states["home"sv]      = assign_value; break;
            case SDLK_PAGEUP:    m_current_key_states["page_up"sv]   = assign_value; break;
            // Escape -> ScrollLock keyboard row
            case SDLK_F1: m_current_key_states["f1"sv]   = assign_value; break;
            case SDLK_F2: m_current_key_states["f2"sv]   = assign_value; break;
            case SDLK_F3: m_current_key_states["f3"sv]   = assign_value; break;
            case SDLK_F4: m_current_key_states["f4"sv]   = assign_value; break;
            case SDLK_F5: m_current_key_states["f5"sv]   = assign_value; break;
            case SDLK_F6: m_current_key_states["f6"sv]   = assign_value; break;
            case SDLK_F7: m_current_key_states["f7"sv]   = assign_value; break;
            case SDLK_F8: m_current_key_states["f8"sv]   = assign_value; break;
            case SDLK_F9: m_current_key_states["f9"sv]   = assign_value; break;
            case SDLK_F10: m_current_key_states["f10"sv] = assign_value; break;
            case SDLK_F11: m_current_key_states["f11"sv] = assign_value; break;
            case SDLK_F12: m_current_key_states["f12"sv] = assign_value; break;
            case SDLK_ESCAPE:      m_current_key_states["escape"sv]       = assign_value; break;
            case SDLK_PRINTSCREEN: m_current_key_states["print_screen"sv] = assign_value; break;
            case SDLK_SCROLLLOCK:  m_current_key_states["scroll_lock"sv]  = assign_value; break;
            case SDLK_PAUSE:       m_current_key_states["pause_break"sv]  = assign_value; break;
            // Tab -> PageDown keyboard_row
            case SDLK_TAB: m_current_key_states["tab"sv] = assign_value; break;
            case SDLK_q: m_current_key_states["q"sv]     = assign_value; break;
            case SDLK_w: m_current_key_states["w"sv]     = assign_value; break;
            case SDLK_e: m_current_key_states["e"sv]     = assign_value; break;
            case SDLK_r: m_current_key_states["r"sv]     = assign_value; break;
            case SDLK_t: m_current_key_states["t"sv]     = assign_value; break;
            case SDLK_y: m_current_key_states["y"sv]     = assign_value; break;
            case SDLK_u: m_current_key_states["u"sv]     = assign_value; break;
            case SDLK_i: m_current_key_states["i"sv]     = assign_value; break;
            case SDLK_o: m_current_key_states["o"sv]     = assign_value; break;
            case SDLK_p: m_current_key_states["p"sv]     = assign_value; break;
            case SDLK_LEFTBRACKET: m_current_key_states["["sv]      = assign_value; break;
            case SDLK_RIGHTBRACKET: m_current_key_states["]"sv]     = assign_value; break;
            case SDLK_RETURN: m_current_key_states["return"sv]      = assign_value; break;
            case SDLK_DELETE: m_current_key_states["delete"sv]      = assign_value; break;
            case SDLK_END: m_current_key_states["end"sv]            = assign_value; break;
            case SDLK_PAGEDOWN: m_current_key_states["page_down"sv] = assign_value; break;
            // CapsLock -> Return keyboard row
            case SDLK_CAPSLOCK: m_current_key_states["caps_lock"sv] = assign_value; break;
            case SDLK_a: m_current_key_states["a"sv] = assign_value; break;
            case SDLK_s: m_current_key_states["s"sv] = assign_value; break;
            case SDLK_d: m_current_key_states["d"sv] = assign_value; break;
            case SDLK_f: m_current_key_states["f"sv] = assign_value; break;
            case SDLK_g: m_current_key_states["g"sv] = assign_value; break;
            case SDLK_h: m_current_key_states["h"sv] = assign_value; break;
            case SDLK_j: m_current_key_states["j"sv] = assign_value; break;
            case SDLK_k: m_current_key_states["k"sv] = assign_value; break;
            case SDLK_l: m_current_key_states["l"sv] = assign_value; break;
            case SDLK_SEMICOLON: m_current_key_states[";"sv] = assign_value; break;
            case SDLK_QUOTE: m_current_key_states["quote"sv] = assign_value; break;
            // LeftShift -> RightShift keyboard row
            case SDLK_LSHIFT: m_current_key_states["lshift"sv] = assign_value; break;
            case SDLK_z: m_current_key_states["z"sv]           = assign_value; break;
            case SDLK_x: m_current_key_states["x"sv]           = assign_value; break;
            case SDLK_c: m_current_key_states["c"sv]           = assign_value; break;
            case SDLK_v: m_current_key_states["v"sv]           = assign_value; break;
            case SDLK_b: m_current_key_states["b"sv]           = assign_value; break;
            case SDLK_n: m_current_key_states["n"sv]           = assign_value; break;
            case SDLK_m: m_current_key_states["m"sv]           = assign_value; break;
            case SDLK_LESS: m_current_key_states["<"sv]        = assign_value; break;
            case SDLK_GREATER: m_current_key_states[">"sv]     = assign_value; break;
            case SDLK_QUESTION: m_current_key_states["?"sv]    = assign_value; break;
            case SDLK_RSHIFT: m_current_key_states["rshift"sv] = assign_value; break;
            case SDLK_UP: m_current_key_states["arrow_up"sv]   = assign_value; break;
            // LeftControl -> RightControl arrow keyboard row
            case SDLK_LCTRL: m_current_key_states["left_control"sv]  = assign_value; break;
            case SDLK_APPLICATION: m_current_key_states["super"sv]   = assign_value; break;
            case SDLK_LALT: m_current_key_states["left_alt"sv]       = assign_value; break;
            case SDLK_SPACE: m_current_key_states["space"sv]         = assign_value; break;
            case SDLK_RALT: m_current_key_states["right_alt"sv]      = assign_value; break;
            case SDLK_RCTRL: m_current_key_states["right_control"sv] = assign_value; break;
            case SDLK_LEFT: m_current_key_states["arrow_left"sv]     = assign_value; break;
            case SDLK_DOWN: m_current_key_states["arrow_down"sv]     = assign_value; break;
            case SDLK_RIGHT: m_current_key_states["arrow_right"sv]   = assign_value; break;
        }
    }
}

[[maybe_unused]] [[nodiscard]] bool SDL::priv::KeyboardState::get_key(const std::string_view& key)
{
    try {
        return m_current_key_states.at(key);
    } catch(const std::out_of_range &exception) {
        // The key does not exist in the table because it was never pressed,
        // so we're just returning false;
        return false;
    }
}
