#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <mutex>
#include <string>

enum class LogLevel { INFO, WARNING, ERROR };

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