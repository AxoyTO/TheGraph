#include <fstream>
#include <string>

namespace uni_cource_cpp {
class Logger {
 public:
  void log(const std::string& string);
  static Logger& get_logger();

 private:
  Logger();
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;

  std::ofstream output_filestream_;
};
}  // namespace uni_cource_cpp
