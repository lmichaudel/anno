#pragma once

#include <fstream>
#include <format>
#include <memory>
#include <string>

enum LogLevel {
  FATAL = 2,
  WARNING = 1,
  DEBUG = 0
};

class Logger {
  LogLevel m_log_level{DEBUG};
  bool m_write_to_stdout{true};
  bool m_write_to_file{false};
  std::ofstream m_log_file;

  inline static std::unique_ptr<Logger> m_instance;

  static std::string log_level_to_string(LogLevel level, const std::string& opening_decorated, const std::string& closing_decorated);
  static std::string log_level_color(LogLevel level);
  void log_raw(LogLevel level, const std::string& message);

  public:
    ~Logger();

    static Logger& instance() {
      if (!m_instance) {
        m_instance = std::make_unique<Logger>();
      }
      return *m_instance;
    }

    void set_log_level(LogLevel level);
    LogLevel get_log_level() const;

    void enable_file_logging(const std::string& file_name, bool clear_on_open = false);
    void disable_file_logging();
    bool is_file_logging_enabled() const;

    void enable_console_logging();
    void disable_console_logging();
    bool is_console_logging_enabled() const;

    template <typename... Args>
    void log(const LogLevel level, const std::string& message, Args&&... args) {
      const std::string formatted_message = std::vformat(message, std::make_format_args(args...));
      log_raw(level, formatted_message);
    }
};

#define LOG_AND_THROW_FATAL(msg, ...) Logger::instance().log(LogLevel::FATAL, msg, ##__VA_ARGS__); throw std::runtime_error(std::format(msg, ##__VA_ARGS__))
#define LOG_FATAL(msg, ...) Logger::instance().log(LogLevel::FATAL, msg, ##__VA_ARGS__)
#define LOG_WARNING(msg, ...) Logger::instance().log(LogLevel::WARNING, msg, ##__VA_ARGS__)
#define LOG_DEBUG(msg, ...) Logger::instance().log(LogLevel::DEBUG, msg, ##__VA_ARGS__)