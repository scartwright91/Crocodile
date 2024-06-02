#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace Crocodile
{
    enum LoggerLevel {
        INFO,
        WARNING,
        ERROR
    };

    static std::string getLevelString(LoggerLevel level) {
        switch (level) {
            case INFO:    return "INFO";
            case WARNING: return "WARNING";
            case ERROR:   return "ERROR";
            default:      return "UNKNOWN";
        }
    }

    static std::string getCurrentTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto nowTimeT = std::chrono::system_clock::to_time_t(now);
        auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::tm nowTm;
        localtime_s(&nowTm, &nowTimeT);
        std::ostringstream oss;
        oss << std::put_time(&nowTm, "%Y-%m-%d %H:%M:%S");
        oss << '.' << std::setfill('0') << std::setw(3) << nowMs.count();
        return oss.str();
    }

    static void LOG(LoggerLevel level, const std::string& message) {
        std::string timestamp = getCurrentTimestamp();
        std::string levelStr = getLevelString(level);
        std::cout << "[" << timestamp << "] [" << levelStr << "] " << message << std::endl;
    }
}