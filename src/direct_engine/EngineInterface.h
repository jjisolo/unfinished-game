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

#include <Engine.h>
#include <EngineRegistry.h>
#include <StartupManager.h>
#include <DirectTextureFactory.h>

// namespace SDL: Holds all SDL2-related classes, interfaces, etc.
namespace SDL
{
	using SmartWindowHandle   = std::unique_ptr<SDL_Window,   decltype(&SDL_DestroyWindow)>;
	using SmartRendererHandle = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
	using SmartTextureHandle  = std::unique_ptr<SDL_Texture,  decltype(&SDL_DestroyTexture)>;

	using BinaryState32 = uint32_t;
	using BinaryState16 = uint16_t;
	using BinaryState8  = uint8_t;

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

	public:
		/*!
		* \brief Get the raw SDL2 renderer pointer
		*/
		SmartRendererHandle get_smart_renderer_handle();

		/*!
		* \brief Get the raw SDL2 renderer pointer
		*/
		SDL_Renderer* get_renderer_handle();

		/*!
		* \brief Start the engine.
		* \sa builtin_on_user_create(), on_user_create()
		*/
		void start();
		
		/*!
		* \brief Start the engine.
		* \sa builtin_on_user_quit(), on_user_quit()
		*/
		void stop();

		/*!
		* \brief The function that is called on the
		* creation of the application
		*
		* Should be inherited and ovverided by the end-user
		* of the engine
		*/
		virtual bool on_user_create();
		
		/*!
		* \brief The function that is called on each update
		* 
		* Should be inherited and ovverided by the end-user
		* of the engine
		*/
		virtual bool on_user_update();

		/*!
		* \brief Resize the logic window.
		*
		* There is two types of resizing the window:
		* logic resize and the acutal resize. The logic
		* resize means updating the window size in the
		* engine registry,the actual resize does the logic
		* resize and the actual resize of the window in the
		* user space
		*
		* \param width  new width of the window
		* \param height new height of the window
		* \param logic_resize apply the logic resize
		*/
		void resize_window(const std::uint32_t width, const std::uint32_t height, const BinaryState16 logic_resize);

		//virtual void on_mouse_move();

	private:
		/*!
		* \brief The function that is called on the
		* creation of the application, as the part of
		* the internal engine structure.
		*/
		void builtin_on_user_create();

		/*!
		* \brief The function that is called on each
		* update, as the part of the internal engine
		* structure.
		*/
		void builtin_on_user_update();

		//void builtin_mousemove_callback();
		//void builtin_windowresize_callback();


	private:
		SmartWindowHandle   m_window_handle;
		SmartRendererHandle m_renderer_handle;
		BinaryState16		m_application_should_close;

		priv::EngineRegistry       m_registry;
		priv::WindowStartupDetails m_window_startup_details;
	};
}