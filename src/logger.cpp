#include "trading/logger.hpp"
#include <ctime>
#include <iostream>

Logger &Logger::getInstance() {
  static Logger instance;
  return instance;
}

void Logger::log(LogLevel level, const std::string &message) {
  std::lock_guard<std::mutex> lock(mtx);

  std::time_t now = std::time(nullptr);
  char timeStr[100];
  std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S",
                std::localtime(&now));

  std::cout << "[" << timeStr << "] [" << getLogLevelString(level) << "] "
            << message << std::endl;
}

std::string Logger::getLogLevelString(LogLevel level) {
  switch (level) {
  case LogLevel::INFO:
    return "INFO";
  case LogLevel::WARNING:
    return "WARNING";
  case LogLevel::ERROR:
    return "ERROR";
  default:
    return "UNKNOWN";
  }
}