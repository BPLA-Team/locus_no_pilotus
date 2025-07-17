// header file:
#include "travelling_salesmans_problem.h"

// std libs:
#include <map>

namespace math {

TravellingSalesmansProblem::TravellingSalesmansProblem(
    AdjacencyMatrix& m, std::size_t num_of_flyers)
    : num_of_flyers_{num_of_flyers} {
  if (num_of_flyers > 1) m.ExtendTo(num_of_flyers);
  paths_stack_.push_back(std::make_shared<TSPNode>(m));
  if (m.GetSize() == 2) CompleteEdgePath_(paths_stack_[0]);
}

void TravellingSalesmansProblem::ExpandStack_() {
  std::pair<std::size_t, std::size_t> value =
      paths_stack_[0]->matrix.GetSelectedValue();
  Edge edge(paths_stack_[0]->matrix.GetSelectedEdge());

  // Первый ребенок, c включением edge
  AdjacencyMatrix with_edge_matrix = paths_stack_[0]->matrix.Reducted();
  with_edge_matrix =
      DeleteEdge_(with_edge_matrix, edge.end_num, edge.start_num);

  // Исключение возможности цикла
  std::map<std::size_t, std::size_t> prev_chains = paths_stack_[0]->chains;
  std::map<std::size_t, std::size_t> new_chains = paths_stack_[0]->chains;
  bool is_new = true;
  for (const auto& [start, end] : prev_chains) {
    if (start == edge.end_num) {
      new_chains.erase(start);
      new_chains[edge.start_num] = end;
      is_new = false;
      break;
    } else if (end == edge.start_num) {
      new_chains[start] = edge.end_num;
      is_new = false;
      break;
    }
  }
  if (is_new) new_chains[edge.start_num] = edge.end_num;
  for (const auto& [start, end] : new_chains) {
    for (const auto& [start2, end2] : new_chains) {
      if (end2 == start) {
        new_chains[start2] = end;
        new_chains.erase(start);
      }
      if (end == start2) {
        new_chains[start] = end2;
        new_chains.erase(start2);
      }
    }
  }
  for (const auto& [start, end] : new_chains) {
    with_edge_matrix = DeleteEdge_(with_edge_matrix, end, start);
  }
  // Исключение возможности цикла

  with_edge_matrix = with_edge_matrix.Minor(value.first, value.second);
  paths_stack_[0]->with_edge = std::make_shared<TSPNode>(
      with_edge_matrix, paths_stack_[0], edge, new_chains);
  if (paths_stack_[0]->with_edge->matrix.GetSize() == 2)
    CompleteEdgePath_(paths_stack_[0]->with_edge);

  // Второй ребенок, c исключением edge
  AdjacencyMatrix without_edge_matrix = paths_stack_[0]->matrix.Reducted();
  without_edge_matrix =
      DeleteEdge_(without_edge_matrix, edge.start_num, edge.end_num);
  paths_stack_[0]->without_edge =
      std::make_shared<TSPNode>(without_edge_matrix, paths_stack_[0]);

  // Добавляем детей в стек вершин,удаляем их родителя
  double with_eval = paths_stack_[0]->with_edge->evaluation;
  double without_eval = paths_stack_[0]->without_edge->evaluation;
  if (!std::isinf(without_eval)) {
    if (FindIndex_(without_eval) < paths_stack_.size())
      paths_stack_.insert(paths_stack_.begin() + FindIndex_(without_eval),
                          paths_stack_[0]->without_edge);
    else
      paths_stack_.push_back(paths_stack_[0]->without_edge);
  }
  if (!std::isinf(with_eval)) {
    if (FindIndex_(with_eval) < paths_stack_.size())
      paths_stack_.insert(paths_stack_.begin() + FindIndex_(with_eval),
                          paths_stack_[0]->with_edge);
    else
      paths_stack_.push_back(paths_stack_[0]->with_edge);
  }
  paths_stack_.erase(paths_stack_.begin());
}

AdjacencyMatrix& TravellingSalesmansProblem::DeleteEdge_(
    AdjacencyMatrix& matrix, std::size_t start_num, std::size_t end_num) {
  for (std::size_t i = 0; i < matrix.GetSize(); ++i) {
    if (std::abs(matrix.GetMatrixValue(i, matrix.GetSize()) -
                 (double)start_num) > precision)
      continue;
    for (std::size_t j = 0; j < matrix.GetSize(); ++j) {
      if (std::abs(matrix.GetMatrixValue(matrix.GetSize(), j) -
                   (double)end_num) > precision)
        continue;
      matrix.SetMatrixValue(i, j, inf);
    }
  }
  return matrix;
}

std::size_t TravellingSalesmansProblem::FindIndex_(double eval) const {
  // Нижняя и верхняя границы
  std::size_t start = 0;
  std::size_t end = paths_stack_.size();
  // Уменьшение области поиска
  while (start < end) {
    std::size_t mid = (start + end) / 2;
    // Если eval нашлось
    if (std::abs(paths_stack_[mid]->evaluation - eval) <= precision)
      if (mid)
        return mid;
      else
        return mid + 1;
    else if (paths_stack_[mid]->evaluation < eval)
      start = mid + 1;
    else
      end = mid;
  }
  if (end)
    return end;
  else
    return end + 1;
}

void TravellingSalesmansProblem::CompleteEdgePath_(
    std::shared_ptr<TSPNode> node) {
  Edge first_missed_edge(std::pair(node->matrix.GetMatrixValue(0, 2), 0));
  Edge second_missed_edge(std::pair(node->matrix.GetMatrixValue(1, 2), 0));
  for (std::size_t i = 0; i < 2; ++i) {
    if ((node->matrix.GetMatrixValue(0, 0) + node->matrix.GetMatrixValue(1, 1) <
         node->matrix.GetMatrixValue(0, 1) +
             node->matrix.GetMatrixValue(1, 0))) {
      first_missed_edge.end_num =
          (std::size_t)node->matrix.GetMatrixValue(2, 0);
      second_missed_edge.end_num =
          (std::size_t)node->matrix.GetMatrixValue(2, 1);
    } else {
      first_missed_edge.end_num =
          (std::size_t)node->matrix.GetMatrixValue(2, 1);
      second_missed_edge.end_num =
          (std::size_t)node->matrix.GetMatrixValue(2, 0);
    }
  }
  node->path.push_back(first_missed_edge);
  node->path.push_back(second_missed_edge);
}

std::vector<std::size_t> TravellingSalesmansProblem::ConvertToVertexPath_() {
  std::map<std::size_t, std::size_t> aux_path;
  for (std::size_t i = 0; i < edge_path_.size(); ++i) {
    aux_path[edge_path_[i].start_num] = edge_path_[i].end_num;
  }
  std::vector<std::size_t> final_path;
  final_path.push_back(0);
  std::size_t key = 0;
  while (aux_path[key] != 0) {
    final_path.push_back(aux_path[key]);
    key = aux_path[key];
  }
  for (std::size_t i = 0; i < final_path.size(); ++i)
    if (final_path[i] > final_path.size() - num_of_flyers_) final_path[i] = 0;
  return final_path;
}

std::vector<std::size_t> TravellingSalesmansProblem::CalculateTrajectory_() {
  while (paths_stack_[0]->matrix.GetSize() > 2) ExpandStack_();
  paths_len_ = paths_stack_[0]->evaluation;
  edge_path_ = paths_stack_[0]->path;
  return ConvertToVertexPath_();
}

}  // namespace math
