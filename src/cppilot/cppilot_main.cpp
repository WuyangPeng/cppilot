#include "cppilot_constant.h"
#include "cppilot_main.h"
#include "common/logging/logger.h"

cppilot::cppilot_main::cppilot_main()
    : base_type{ cppilot_type }
{
}

void cppilot::cppilot_main::create_initializer(const celeritas::command_line_config& command_line_config)
{
}


int main(const int argc, char** argv)
{
    try
    {
        cppilot::cppilot_main main{};

        main.run(argc, argv);
    }
    catch (const std::exception& error)
    {
        LOG_CHANNEL(celeritas::default_channel, error) << "error:" << error.what();
        return 1;
    }
    catch (...)
    {
        LOG_CHANNEL(celeritas::default_channel, fatal) << "unknown error.";
        return 1;
    }

    return 0;
}