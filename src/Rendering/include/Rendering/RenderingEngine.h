#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <spdlog/spdlog.h>

#include <Rendering/RenderingExceptions.h>

#include <unordered_map>
#include <type_traits>
#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <variant>
#include <string>
#include <memory>
#include <vector>
#include <list>

#define VectorFind(vector, item)         (std::find((vector).begin(), (vector).end(), (item)))
#define VectorFindBinary(vector, item)   ((VectorFind((vector), (item)) != (vector).end()))

namespace SDL {
    static constexpr const SDL_Rect NULL_RECT = {0, 0, 0, 0};
}


