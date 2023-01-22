#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <unordered_map>
#include <type_traits>
#include <algorithm>
#include <stdexcept>
#include <variant>
#include <string>
#include <memory>
#include <vector>

#define VectorFind(vector, item)         (std::find((vector).begin(), (vector).end(), (item)))
#define VectorFindBinary(vector, item)   ((VectorFind((vector), (item)) != (vector).end()))
#define VectorFindSuccess(iter, vector)  ((iter) != (vector).end())

namespace SDL
{
	using SmartTextureHandle  = std::unique_ptr<SDL_Texture,  decltype(&SDL_DestroyTexture)>;
	using SmartRendererHandle = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
}