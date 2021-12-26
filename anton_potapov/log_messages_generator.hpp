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
};
}  // namespace uni_cource_cpp
