#pragma once

#include <format>
#include <stdexcept>
#include <string>

namespace celeritas
{
    class cppilot_error : public std::runtime_error
    {
    public:
        using class_type = cppilot_error;
        using base_type = std::runtime_error;

        template <typename... Args>
        explicit cppilot_error(std::format_string<Args...> fmt, Args&&... args)
            : base_type{ std::vformat(fmt.get(), std::make_format_args(args...)) }
        {
        }

        explicit cppilot_error(const std::string& error);
    };
}
