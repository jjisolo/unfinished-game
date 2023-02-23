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
#include <utility>

#include "MouseState.h"
#include "KeyboardState.h"
#include "EngineRegistry.h"
#include "StartupManager.h"

#include <Rendering/RenderingManager.h>

/*!
 * This is a global namespace for all public objects
 * that are located in the game engine, feel free to use
 * any of them.
 */
namespace SDL
{
    using SmartWindowHandle      = std::unique_ptr<SDL_Window,   decltype(&SDL_DestroyWindow)>;
    using SmartRendererHandle    = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
    using DirectWindowPosition   = std::pair<int, int>;
    using DirectWindowDimensions = std::pair<int, int>;
    using BinaryState16          = uint16_t;

    /*!
    * This is internal namespace for the game engine, you should not use
    * any of the data structres, modify any data that is under that namespace
    */
    namespace priv
    {
        static constexpr const char* default_window_title     = "Dachshund adventures!";
        static constexpr const int   default_window_flags     = SDL_WINDOW_SHOWN;
        static constexpr const int   default_renderer_flags   = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
        static constexpr const int   default_rendering_device = -1;

        static constexpr DirectWindowPosition   default_window_position   = {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED};
        static constexpr DirectWindowDimensions default_window_dimensions = {640, 480};
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

        explicit EngineInterface(std::shared_ptr<SDL::RenderingManager> render_manager) :
            m_binded_render_manager (std::move(render_manager)),
            m_window_handle         (nullptr, SDL_DestroyWindow),
            m_renderer_handle       (nullptr, SDL_DestroyRenderer)
        {
            m_keyboard_state.reset(new SDL::priv::KeyboardState());
            m_mouse_state   .reset(new SDL::priv::MouseState());

            builtin_on_user_create();
            on_user_create();
        }

    public:
        /*!
        * \brief Get the raw SDL2 renderer pointer
        */
            SDL_Renderer* get_renderer_handle();

            /*!
            * \brief Get the position of the application root window
            */
            [[maybe_unused]] DirectWindowPosition get_window_position();

            /*!
             * \brief Get the dimensions of the application root window
             */
            [[maybe_unused]] DirectWindowDimensions get_window_dimensions();

            /*!
             * \brief Start the engine.
             *
             * \sa builtin_on_user_create(), on_user_create()
             */
            void start();

            /*!
             * \brief Start the engine.
             *
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
            [[maybe_unused]] void resize_window(std::uint32_t width, std::uint32_t height, BinaryState16 logic_resize);

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

        private:
            SDL::SmartWindowHandle   m_window_handle;
            SDL::SmartRendererHandle m_renderer_handle;
            SDL::BinaryState16		 m_application_should_close{};

            SDL::priv::EngineRegistry         m_registry;

            std::shared_ptr<SDL::priv::MouseState>     m_mouse_state;
            std::shared_ptr<SDL::priv::KeyboardState>  m_keyboard_state;
            std::shared_ptr<RenderingManager>          m_binded_render_manager;
    };
}
