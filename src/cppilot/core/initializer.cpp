#include "initializer.h"
#include "common/core/noexcept_safe_call_and_log.h"
#include "common/logging/logger.h"
#include "common/system/crash.h"
#include "config/config_constant.h"
#include "cppilot/cppilot_constant.h"

#include <boost/polymorphic_pointer_cast.hpp>

cppilot::initializer::initializer_shared_ptr cppilot::initializer::create(const std::string_view& main_type, std::string config_file_path)
{
    return std::make_shared<initializer>(main_type, std::move(config_file_path));
}

cppilot::initializer::initializer(const std::string_view& main_type, std::string config_file_path)
    : main_type_{ main_type },
      current_path_{ boost::filesystem::current_path() },
      io_context_{},
      work_guard_{ boost::asio::make_work_guard(io_context_) },
      daemon_{ std::make_unique<celeritas::daemon>(main_type_) },
      signals_{ io_context_, SIGINT, SIGTERM }
{
}

void cppilot::initializer::initialize()
{
    setup_signal_handler();
    initialize_default_logger();
}

void cppilot::initializer::run()
{
    LOG_CHANNEL(cppilot_channel, info) << main_type_ << " is start";

    for (;;)
    {
        try
        {
            io_context_.run();
            break;
        }
        catch (const std::exception& error)
        {
            LOG_CHANNEL(cppilot_channel, error) << "io context error: " << error.what();
        }
        catch (...)
        {
            LOG_CHANNEL(cppilot_channel, fatal) << "io context fatal error.";
        }
    }

    LOG_CHANNEL(cppilot_channel, info) << main_type_ << " io context is stop";
}

void cppilot::initializer::initialize_default_logger()
{
    celeritas::logger::init_global(celeritas::severity_level_type::trace);
    celeritas::logger::init_console(celeritas::severity_level_type::trace);

    celeritas::logger::init_file(cppilot_channel.data(),
                                 cppilot_channel.data(),
                                 celeritas::severity_level_type::trace,
                                 celeritas::default_logger_rotation_size,
                                 true);
}

void cppilot::initializer::setup_signal_handler()
{
    // 异步等待信号
    signals_.async_wait(
        [self = boost::polymorphic_pointer_downcast<class_type>(shared_from_this())](const boost::system::error_code& error, const int signal_number) {
            if (!error)
            {
                LOG_CHANNEL(cppilot_channel, info)
                << self->main_type_
                << " is stop! signal_number = "
                << signal_number
                << ",error = "
                << error.message();

                celeritas::noexcept_safe_call_and_log([self = self] {
                                                          self->stop();
                                                      },
                                                      cppilot_channel,
                                                      "setup signal handler error: ");
            }
        });

    celeritas::crash::set_signal();
}

void cppilot::initializer::stop()
{
    daemon_.reset();
    work_guard_.reset();

    LOG_CHANNEL(cppilot_channel, info) << main_type_ << " is stop finish!";
}

