#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

#include "log_messages_generator.hpp"

namespace {
std::string get_current_date_time() {
  const auto date_time = std::chrono::system_clock::now();
  const auto date_time_t = std::chrono::system_clock::to_time_t(date_time);
  std::stringstream date_time_string;
  date_time_string << std::put_time(std::localtime(&date_time_t),
                                    "%Y.%m.%d %H:%M:%S");
  return date_time_string.str();
}
}  // namespace

namespace uni_cource_cpp {
std::string LogMessagesGenerator::generation_started_string(int id) {
  std::stringstream generation_started_stringstream;
  generation_started_stringstream << get_current_date_time() << ": Graph " << id
                                  << ", Generation Started" << std::endl;
  return generation_started_stringstream.str();
}

std::string LogMessagesGenerator::generation_finished_string(
    int id,
    const std::string& graph_description) {
  std::stringstream generation_finished_stringstream;
  generation_finished_stringstream << get_current_date_time() << ": Graph "
                                   << id << ", Generation Finished "
                                   << graph_description << std::endl;
  return generation_finished_stringstream.str();
}

std::string LogMessagesGenerator::generation_max_depth_warning(
    int id,
    int max_generated_depth,
    int given_depth) {
  std::stringstream generation_max_depth_warning_stringstream;
  generation_max_depth_warning_stringstream
      << get_current_date_time() << ": Graph " << id
      << "generated graph's depth=" << max_generated_depth
      << " is less than specified one =" << given_depth << std::endl;
  return generation_max_depth_warning_stringstream.str();
}
}  // namespace uni_cource_cpp
