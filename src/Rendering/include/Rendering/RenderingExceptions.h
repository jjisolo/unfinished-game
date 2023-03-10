#pragma once

#include <exception>

namespace SDL
{
    namespace priv
    {
        static constexpr auto DirectInvalidArgument_What = R"(
            The DirectInvalidArgument() exception is thrown whenever the invalid
            argument is passed to the function.

            Consider reading API, that is located in the $PROJECT/doc folder,
            and can be generated by the doxygen, of the function that invoked
            this error.
        )";

        static constexpr auto DirectSystemException_What = R"(
            The DirectSystemException() exception is thrown whenever something
            failed in the SDL2 internally

            Consider reading API, that is located in the $PROJECT/doc folder,
            and can be generated by the doxygen, of the function that invoked
            this error.
        )";

        static constexpr auto DirectImageSystemException_What = R"(
            The DirectImageSystemException() exception is thrown whenever something
            failed in the SDL2_image internally

            Consider reading API, that is located in the $PROJECT/doc folder,
            and can be generated by the doxygen, of the function that invoked
            this error.
        )";

        static constexpr auto DirectMixerSystemException_What = R"(
            The DirectMixerSystemException() exception is thrown whenever something
            failed in the SDL2_ttf internally

            Consider reading API, that is located in the $PROJECT/doc folder,
            and can be generated by the doxygen, of the function that invoked
            this error.
        )";

        static constexpr auto DirectTrueTypeSystemException_What = R"(
            The DirectTrueTypeSystemException() exception is thrown whenever something
            failed in the SDL2_ttf internally

            Consider reading API, that is located in the $PROJECT/doc folder,
            and can be generated by the doxygen, of the function that invoked
            this error.
        )";
    }

    struct [[maybe_unused]] DirectInvalidArgument : public std::exception
    {
        [[nodiscard]] const char* what () const noexcept override
        {
            return SDL::priv::DirectInvalidArgument_What;
        }
    };

    struct [[maybe_unused]] DirectSystemException : public std::exception
    {
        [[nodiscard]] const char* what () const noexcept override
        {
            return SDL::priv::DirectSystemException_What;
        }
    };

    struct [[maybe_unused]] DirectImageSystemException : public std::exception
    {
        [[nodiscard]] const char* what () const noexcept override
        {
            return SDL::priv::DirectImageSystemException_What;
        }
    };

    struct [[maybe_unused]] DirectMixerSystemException : public std::exception
    {
        [[nodiscard]] const char* what () const noexcept override
        {
            return SDL::priv::DirectMixerSystemException_What;
        }
    };

    struct [[maybe_unused]] DirectTrueTypeSystemException : public std::exception
    {
        [[nodiscard]] const char* what () const noexcept override
        {
            return SDL::priv::DirectTrueTypeSystemException_What;
        }
    };
}

