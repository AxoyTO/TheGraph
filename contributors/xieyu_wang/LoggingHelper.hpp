#include "Graph.hpp"
#include "Logger.hpp"
namespace uni_course_cpp {
class LoggingHelper {
 public:
  static std::string getTimeByString();
  static void logStart(Logger& logger, int graphIndex);
  static void logColors(Logger& logger, Graph graph);
  static void logEnd(Logger& logger, Graph graph, int graphIndex);
};
}  // namespace uni_course_cpp
