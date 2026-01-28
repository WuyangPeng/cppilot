#pragma once

#include "common/framework/celeritas_main.h"

namespace cppilot
{
    class cppilot_main final : public celeritas::celeritas_main
    {
    public:
        using class_type = cppilot_main;
        using base_type = celeritas_main;

        cppilot_main();

    private:
        void create_initializer(const celeritas::command_line_config& command_line_config) override;
    };
}