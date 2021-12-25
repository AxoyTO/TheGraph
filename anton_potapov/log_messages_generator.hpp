#include <string>

namespace uni_cource_cpp {
class LogMessagesGenerator {
 public:
  static std::string generation_started_string(int id);

  static std::string generation_finished_string(
      int id,
      const std::string& graph_description);

  static std::string generation_max_depth_warning(int id,
                                                  int max_generated_depth,
                                                  int given_depth);
  static std::string traversal_started_string(int index);
  static std::string traversal_finished_string(int index,
                                               const std::string& paths);
};
}  // namespace uni_cource_cpp
