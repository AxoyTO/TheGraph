#include "logger.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>

namespace uni_cource_cpp {

void Logger::set_output_file_path(const std::optional<std::string>& file_path) {
  if (!file_path.has_value()) {
    if (output_stream_.has_value()) {
      output_stream_.value().close();
    }
    output_stream_ = std::nullopt;
    return;
  }

  output_stream_ = std::ofstream(file_path.value());
  if (!output_stream_->is_open()) {
    throw std::runtime_error("Failed to create file stream");
  }
}

void Logger::log(const std::string& text) {
  std::cout << text << std::endl;
  if (output_stream_.has_value()) {
    output_stream_.value() << text << std::endl;
  }
}

}  // namespace uni_cource_cpp
