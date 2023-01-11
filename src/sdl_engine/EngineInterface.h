// InterfaceWindow.h: Base window class that is needed to be inherited
// by a user.
#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <cstdint>
#include <memory>
#include <string>

#include <StartupManager.h>

// namespace SDL: Holds all SDL2-related classes, interfaces, etc.
namespace SDL
{
	namespace priv
	{
		static constexpr const char* default_window_title     = "Dachshund adventures!";
		static constexpr const int   default_window_flags     = SDL_WINDOW_SHOWN;
		static constexpr const int   default_renderer_flags   = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
		static constexpr const int   default_rendering_device = -1;

		static constexpr std::pair<int, int>default_window_position   = {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED};
		static constexpr std::pair<int, int>default_window_dimensions = {640, 480};
	}

	class EngineInterface
	{
		using SmartWindowHandle   = std::unique_ptr<SDL_Window,   decltype(&SDL_DestroyWindow)>;
		using SmartRendererHandle = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
		using SmartTextureHandle  = std::unique_ptr<SDL_Texture,  decltype(&SDL_DestroyTexture)>;

		using BinaryState16       = uint16_t;

	public: 
		EngineInterface(EngineInterface&& engine_interface) noexcept:
			m_window_handle  (std::move(engine_interface.m_window_handle)),
			m_renderer_handle(std::move(engine_interface.m_renderer_handle))
		{
			builtin_on_user_create();
			on_user_create();
		}
	
		EngineInterface() :
			m_window_handle  (nullptr, SDL_DestroyWindow), 
			m_renderer_handle(nullptr, SDL_DestroyRenderer) 
		{
			builtin_on_user_create();
			on_user_create();
		}

	private:
		void builtin_on_user_create();
		void builtin_on_user_update();

		//void builtin_mousemove_callback();
		//void builtin_windowresize_callback();

	public:
		void start();
		void stop();


		virtual bool on_user_create();
		virtual bool on_user_update();

		//virtual void on_mouse_move();

	private:
		SmartWindowHandle   m_window_handle;
		SmartRendererHandle m_renderer_handle;
		BinaryState16		m_application_should_close;

		priv::WindowStartupDetails m_window_startup_details;
	};
}