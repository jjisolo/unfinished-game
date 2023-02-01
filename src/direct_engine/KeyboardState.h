#pragma once

#include <SDL.h>
#include <array>
#include <cstdint>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace SDL::priv
{
    class KeyboardState
    {
        using KeyStates = std::unordered_map<std::string_view, bool>;
    public:

        /*! \brief Update the keyboard key state */
        [[maybe_unused]] void update(SDL_Event& event);

        /*! \brief Get the specific key state */
        [[maybe_unused]] [[nodiscard]] bool get_key(const std::string_view& key);

    private:
        KeyStates m_current_key_states;
    };
}
