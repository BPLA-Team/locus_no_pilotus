#pragma once

// std libs:
#include <map>

// our code libs:
#include "obstacles.h"

namespace math {

/// @brief Вершина графа
struct PathWayNode {
  /**
   * @brief PathWayNode
   * @param p координаты
   * @param n номер вершины
   */
  PathWayNode(Point p, std::size_t n, bool is_const)
      : point{p}, number{n}, is_visited{false}, is_constant{is_const} {}

  // Ребра данной вершины
  std::vector<std::shared_ptr<PathWayNode>> edges;

  // Длины ребер
  std::vector<double> edges_lens;

  std::shared_ptr<CircleObstacle> circle_ptr = nullptr;

  std::shared_ptr<PolygonObstacle> poly_ptr = nullptr;

  // Координаты вершины
  Point point;

  // Номер вершины
  std::size_t number;

  // Была ли уже посещена вершина
  // в алгоритме Дейкстры
  bool is_visited;

  // Явлеяется ли точка постоянной в графе
  bool is_constant;
};

/// @brief Граф вершин между контрольными точками
struct PathWayGraph {
  // Вершины графа
  std::vector<std::shared_ptr<PathWayNode>> nodes;

  // Добавить новую вершину
  void AddNode(PathWayNode* new_node) { nodes.emplace_back(new_node); }

  // Удалить временные вершины
  void RemoveNonConstantNodes() {
    std::size_t i = 0;
    while (i < nodes.size()) {
      if (nodes[i]->is_constant) {
        ++i;
        continue;
      }
      for (const auto& other_node : nodes[i]->edges) {
        std::size_t j = 0;
        while (j < other_node->edges.size()) {
          if (nodes[i]->point != other_node->edges[j]->point) {
            ++j;
            continue;
          }
          other_node->edges.erase(other_node->edges.begin() + j);
          other_node->edges_lens.erase(other_node->edges_lens.begin() + j);
          break;
        }
      }
      nodes.erase(nodes.begin() + i);
    }
  }

  // Добавить новое ребро
  void AddEdge(std::size_t node_1, std::size_t node_2, double length) {
    std::shared_ptr<PathWayNode> node_ptr1, node_ptr2;

    for (auto node : nodes) {
      if (node->number == node_1)
        node_ptr1 = node;
      else if (node->number == node_2)
        node_ptr2 = node;
    }

    node_ptr1->edges.push_back(node_ptr2);
    node_ptr1->edges_lens.push_back(length);
    node_ptr2->edges.push_back(node_ptr1);
    node_ptr2->edges_lens.push_back(length);
  }
};

/// @brief Реализация алгоритма Дейкстры
class DijkstrasAlgorithm {
 public:
  /**
   * @brief Инициализирует новый экземпляр Dijkstras_algorithm
   * @param start: начальная точка
   * @param end: конечная точка
   */
  DijkstrasAlgorithm(PathWayGraph graph)
      : first_point_{graph.nodes.size() - 2},
        second_point_{graph.nodes.size() - 1},
        path_nodes_{graph.nodes},
        min_length_{0} {
    graphs_vertex_[first_point_] = 0;
    graphs_vertex_[second_point_] = inf;
    CalculateMinPath_();
  }

  // Возвращает длину кратчайшего пути
  double Get_Min_Len() const { return min_length_; }

  // Возвращает кратчайший путь
  std::vector<std::size_t> Get_Min_Path() const { return min_path_; }

 private:
  // Номер первой точки
  std::size_t first_point_;

  // Номер второй точки
  std::size_t second_point_;

  // Все вершины графа
  std::vector<std::shared_ptr<PathWayNode>> path_nodes_;

  // Длина кратчайшего пути из start_ в end_
  double min_length_;

  // Кратчайший маршрут из start_ в end_
  std::vector<std::size_t> min_path_;

  // Кратчайшие найденные расстояния до рассматриваемых вершин
  std::map<std::size_t, double> graphs_vertex_;

  /**
   * @brief Определяет длину кратчайшего пути из start_ в end_
   */
  void CalculateMinPath_();
};

}  // namespace math
