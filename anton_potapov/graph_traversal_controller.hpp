#include <functional>

#include "concurrent_controller.hpp"
#include "graph.hpp"

namespace uni_cource_cpp {
class GraphTraversalController : public ConcurrentController {
 public:
  using TraversalStartedCallback = std::function<void(int index, Graph& graph)>;
  using TraversalFinishedCallback = std::function<
      void(int index, Graph& graph, std::vector<GraphPath> paths)>;

  GraphTraversalController(int threads_count, std::map<int, Graph>& graphs);

  void traverse(const TraversalStartedCallback& traversalStartedCallback,
                const TraversalFinishedCallback& traversalFinishedCallback);

 private:
  void init_jobs(const TraversalStartedCallback& gen_started_callback,
                 const TraversalFinishedCallback& gen_finished_callback);
};
}  // namespace uni_cource_cpp
