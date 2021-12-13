#include <string>

namespace uni_cource_cpp {
class Logger {
 public:
  void log(const std::string& string);
  static Logger& get_instance();

 private:
  Logger() = default;
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(Logger&&) = delete;
};
}  // namespace uni_cource_cpp