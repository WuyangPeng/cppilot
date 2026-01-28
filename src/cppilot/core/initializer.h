#pragma once

#include "common/system/daemon.h"

#include <boost/filesystem.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>

#include <memory>
#include <string>

namespace cppilot
{
    class initializer : public std::enable_shared_from_this<initializer>
    {
    public:
        using class_type = initializer;
        using initializer_shared_ptr = std::shared_ptr<initializer>;

        [[nodiscard]] static initializer_shared_ptr create(const std::string_view& main_type, std::string config_file_path);

        initializer(const std::string_view& main_type, std::string config_file_path);

        ~initializer() noexcept = default;

        initializer(const initializer& rhs) noexcept = delete;

        initializer& operator=(const initializer& rhs) noexcept = delete;

        initializer(initializer&& rhs) noexcept = delete;

        initializer& operator=(initializer&& rhs) noexcept = delete;

        void initialize();

        void run();

    private:
        using path_type = boost::filesystem::path;
        using io_context_type = boost::asio::io_context;
        using executor_type = boost::asio::io_context::executor_type;
        using executor_work_guard_type = boost::asio::executor_work_guard<executor_type>;
        using signal_set_type = boost::asio::signal_set;
        using daemon_unique_ptr = std::unique_ptr<celeritas::daemon>;

        void initialize_default_logger();

        // 设置信号处理
        void setup_signal_handler();

        void stop();

        std::string main_type_;
        path_type current_path_;
        io_context_type io_context_;
        executor_work_guard_type work_guard_;
        daemon_unique_ptr daemon_;

        // 信号集成员变量
        signal_set_type signals_;
    };
}