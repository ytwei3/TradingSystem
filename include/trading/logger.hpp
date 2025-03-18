#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <chrono>
#include <mutex>
#include <print>
#include <string>

enum class LogLevel { INFO, WARNING, ERROR };

inline auto getLogLevelString(LogLevel level) -> std::string {
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

// CRTP
template <typename Derived> class LoggerBase {
public:
  void log(LogLevel level, const std::string &message) {
    static_cast<Derived *>(this)->logImpl(level, message);
  }
};

class ConsoleLogger : public LoggerBase<ConsoleLogger> {
public:
  void logImpl(LogLevel level, const std::string &message) {
    std::print("[{}] [{}] {}\n", std::chrono::system_clock::now(),
               getLogLevelString(level), message);
  }
};

class FileLogger : public LoggerBase<FileLogger> {
public:
  void logImpl(LogLevel level, const std::string &message);
};

class Logger {
public:
  static Logger &getInstance();
  void log(LogLevel level, const std::string &message);

private:
  Logger() = default;
  ~Logger() = default;
  Logger(const Logger &) = delete;
  Logger &operator=(const Logger &) = delete;

  std::mutex mtx;
  std::string getLogLevelString(LogLevel level);
};

#endif // LOGGER_HPP