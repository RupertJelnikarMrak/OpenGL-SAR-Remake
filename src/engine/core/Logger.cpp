#include "engine/core/Logger.hpp"

#include "utils/PathUtils.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>
#include <chrono>
#include <string>

std::string currentTime() {
    auto now = std::chrono::system_clock::now();

    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm* local_time = std::localtime(&currentTime);

    char buffer[80];

    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", local_time);

    std::string formattedTime(buffer);
    return formattedTime;
}

void initSpdlog() {
    const char *logsPath = PathUtils::absolutePath("/logs/");

    if (!std::filesystem::exists(logsPath))
        std::filesystem::create_directory(logsPath);

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_st>(std::string(logsPath) + "log_" + currentTime() + ".log"));

    auto logger = std::make_shared<spdlog::logger>("main_logger", begin(sinks), end(sinks));

    spdlog::set_default_logger(logger);
    spdlog::flush_on(spdlog::level::debug);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");

    spdlog::info("Logger initialized");
}
