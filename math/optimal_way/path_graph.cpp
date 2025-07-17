// header file
#include "path_graph.h"

// std libs:
#include <algorithm>

namespace math {

void DijkstrasAlgorithm::CalculateMinPath_() {
  while (graphs_vertex_[second_point_] > min_length_) {
    std::shared_ptr<PathWayNode> min_len_key;
    for (auto& elem : graphs_vertex_)
      if ((std::abs(elem.second - min_length_) <= precision) &&
          (!path_nodes_[elem.first]->is_visited))
        min_len_key = path_nodes_[elem.first];

    for (std::size_t i = 0; i < min_len_key->edges.size(); ++i)
      if ((graphs_vertex_.find(min_len_key->edges[i]->number) ==
           graphs_vertex_.end()) ||
          (graphs_vertex_[min_len_key->edges[i]->number] >
           graphs_vertex_[min_len_key->number] + min_len_key->edges_lens[i]))
        graphs_vertex_[min_len_key->edges[i]->number] =
            graphs_vertex_[min_len_key->number] + min_len_key->edges_lens[i];
      else
        continue;
    min_len_key->is_visited = true;

    min_length_ = inf;
    for (auto& elem : graphs_vertex_)
      if ((elem.second < min_length_) &&
          (!(*path_nodes_[elem.first]).is_visited))
        min_length_ = elem.second;
  }

  // Определение маршрута по длинам, сохранившимся в graphs_vertex_
  std::size_t end = second_point_;
  min_path_.push_back(end);
  while (end != first_point_) {
    for (std::size_t i = 0; i < path_nodes_[end]->edges.size(); ++i)
      if (std::abs(graphs_vertex_[path_nodes_[end]->number] -
                   graphs_vertex_[path_nodes_[end]->edges[i]->number] -
                   path_nodes_[end]->edges_lens[i]) <= precision) {
        end = path_nodes_[end]->edges[i]->number;
        min_path_.push_back(end);
        break;
      }
  }
  std::reverse(min_path_.begin(), min_path_.end());
}

}  // namespace math
