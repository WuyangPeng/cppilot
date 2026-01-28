#include "cppilot/cppilot_constant.h"
#include "cppilot_main.h"
#include "common/core/command_line_config.h"
#include "common/logging/logger.h"
#include "cppilot/core/initializer.h"

cppilot::cppilot_main::cppilot_main()
    : base_type{ cppilot_type }
{
}

void cppilot::cppilot_main::create_initializer(const celeritas::command_line_config& command_line_config)
{
    const auto  context = initializer::create(get_server_type(), command_line_config.get<std::string>(celeritas::config_file_path_command_line.data()));

    context->initialize();
    context->run();
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