#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "Crocodile/Core.h"

namespace Crocodile
{
    enum LoggerLevel {
        INFO,
        WARNING,
        ERROR
    };

    static const char* getColorCode(LoggerLevel level) {
        switch (level) {
            case INFO:    return "\033[32m"; // Green
            case WARNING: return "\033[33m"; // Yellow 
            case ERROR:   return "\033[31m"; // Red
            default:      return "\033[0m";  // Reset
        }
    }

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

    static void CROCODILE_API LOG(LoggerLevel level, const std::string& message) {
        std::string timestamp = getCurrentTimestamp();
        std::string levelStr = getLevelString(level);
        std::cout << "[" << timestamp << "] " 
                 << getColorCode(level) << "[" << levelStr << "]" << "\033[0m" 
                 << " " << message << std::endl;
    }
}