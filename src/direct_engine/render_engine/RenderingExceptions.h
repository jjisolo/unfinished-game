#pragma once

#include <exception>

namespace SDL
{
    struct [[maybe_unused]] DirectSystemException : public std::exception
    {
        [[nodiscard]] const char* what () const throw ()
        {
            return "DirectSystemException";
        }
    };

    struct [[maybe_unused]] DirectImageSystemException : public std::exception
    {
        [[nodiscard]] const char* what () const throw ()
        {
            return "DirectImageSystemException";
        }
    };

    struct [[maybe_unused]] DirectMixerSystemException : public std::exception
    {
        [[nodiscard]] const char* what () const throw ()
        {
            return "DirectMixerSystemException";
        }
    };

    struct [[maybe_unused]] DirectTrueTypeSystemException : public std::exception
    {
        [[nodiscard]] const char* what () const throw ()
        {
            return "DirectTrueTypeSystemException";
        }
    };

    struct [[maybe_unused]] DirectRuntimeException : public std::exception
    {
        [[nodiscard]] const char* what () const throw ()
        {
            return "DirectRuntimeException";
        }
    };

    struct [[maybe_unused]] DirectImageRuntimeException : public std::exception
    {
        [[nodiscard]] const char* what () const throw ()
        {
            return "DirectImageRuntimeException";
        }
    };

    struct [[maybe_unused]] DirectMixerRuntimeException : public std::exception
    {
        [[nodiscard]] const char* what () const throw ()
        {
            return "DirectMixerRuntimeException";
        }
    };

    struct [[maybe_unused]] DirectTrueTypeRuntimeException : public std::exception
    {
        [[nodiscard]] const char* what () const throw ()
        {
            return "DirectTrueTypeRuntimeException";
        }
    };
}

