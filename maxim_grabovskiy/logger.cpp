#include "logger.hpp"
#include <string>
namespace uni_course_cpp {
void Logger::log(const std::string& string) {
  std::cout << get_current_date_time() << " ";
  std::cout << " " << string;
  std::cout << "\n";
}
}  // namespace uni_course_cpp
