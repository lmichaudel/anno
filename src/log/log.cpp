#include "log/log.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

Logger::~Logger() {
  disable_file_logging();
}

void Logger::set_log_level(const LogLevel level) {
  m_log_level = level;
}

LogLevel Logger::get_log_level() const {
  return m_log_level;
}

void Logger::enable_file_logging(const std::string& file_name, const bool clear_on_open) {
  if (m_log_file.is_open()) {
    m_log_file.close();
  }

  if (file_name.empty()) {
    log(WARNING, "Incorrect path given to set_log_file_path. As a result 'write_to_file' was set to false.");
    disable_file_logging();
    return;
  }

  m_log_file.open(file_name, std::ios::app);

  if (!m_log_file.is_open()) {
    log(WARNING, "Could not open log file: " + file_name + ". As a result, 'write_to_file' was set to false.");
    disable_file_logging();
  } else {
    m_write_to_file = true;
    if (clear_on_open) {
      m_log_file.close();
      m_log_file.open(file_name, std::ios::out | std::ios::trunc);
      m_log_file.close();
      m_log_file.open(file_name, std::ios::out | std::ios::app);
    }
  }
}

void Logger::disable_file_logging() {
  m_write_to_file = false;
  if (m_log_file.is_open()) {
    m_log_file.close();
  }
}

bool Logger::is_file_logging_enabled() const {
  return m_write_to_file;
}

void Logger::enable_console_logging() {
  m_write_to_stdout = true;
}

void Logger::disable_console_logging() {
  m_write_to_stdout = false;
}

bool Logger::is_console_logging_enabled() const {
  return m_write_to_stdout;
}

std::string Logger::log_level_to_string(const LogLevel level, const std::string& opening_decorated, const std::string& closing_decorated) {
  switch (level) {
  case FATAL:
    return opening_decorated + "FATAL" + closing_decorated + "  ";
  case WARNING:
    return opening_decorated + "WARNING" + closing_decorated;
  case DEBUG:
    return opening_decorated + "DEBUG" + closing_decorated + "  ";
  default:
    return "UNKNOWN";
  }
}

std::string Logger::log_level_color(const LogLevel level) {
  switch (level) {
  case WARNING:
    return "\033[33m"; // yellow
  case DEBUG:
    return "\033[36m"; // green
  default:
    LOG_WARNING("Unknown log level !");
    return "";
  }
}

std::string get_timestamp() {
  const auto now = std::chrono::system_clock::now();
  const auto time_t = std::chrono::system_clock::to_time_t(now);
  const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                      now.time_since_epoch()) %
                  1000;

  std::stringstream ss;
  ss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
  ss << "." << std::setfill('0') << std::setw(3) << ms.count();
  return ss.str();
}

void Logger::log_raw(const LogLevel level, const std::string& message) {
  if (level < m_log_level)
    return;

  std::stringstream ss;
  ss << log_level_to_string(level, "[", "] ");
  ss << "[" << get_timestamp() << "] ";
  ss << message;
  ss << "\033[0m";

  const std::string final_message = ss.str();

  if (m_write_to_stdout) {
    if (level == FATAL)
      std::cerr << final_message << std::endl;
    else
      std::cout << log_level_color(level) << final_message << std::endl;
  }

  if (m_write_to_file) {
    if (!m_log_file.is_open()) {
      disable_file_logging();
      log(FATAL, "File logging was enabled, but the log file is invalid.");
      return;
    }

    m_log_file << final_message << std::endl;
    m_log_file.flush();
  }
}