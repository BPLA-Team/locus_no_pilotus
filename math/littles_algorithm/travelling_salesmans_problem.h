#pragma once

// our code libs:
#include "adjacency_matrix.h"
#include "tspgraph.h"

namespace math {

/// @brief Решение задачи коммивояжера
class TravellingSalesmansProblem {
 public:
  /**
   *  @brief Инициализирует новый экземпляр TravellingSalesmansProblem для
   * нескольких коммивояжеров
   *  @param matrix: матрица смежности графа
   *  @param num_of_flyers: количество коммивояжеров
   */
  TravellingSalesmansProblem(AdjacencyMatrix& matrix,
                             std::size_t num_of_flyers = 1);

  // Возвращает длину оптимального маршрута
  double GetTrajLength() const { return paths_len_; }

  // Возвращает оптимальный маршрут для данной задачи
  inline std::vector<std::size_t> GetTrajectory() {
    return CalculateTrajectory_();
  }

 private:
  // Количество коммивояжеров
  std::size_t num_of_flyers_{1};

  // Вектор с указателями на вершины графа
  // Отсортирован в порядке возрастания нижней оценки
  std::vector<std::shared_ptr<TSPNode>> paths_stack_;

  // Длина получившегося маршрута
  double paths_len_;

  // Ребра получившегося маршрута
  std::vector<Edge> edge_path_;

  // Вспомогательные методы для работы с paths_stack_
  /**
   * @brief Заменяет вершину графа в path_stack_ на её детей,
   * без нарушения порядка
   */
  void ExpandStack_();

  /**
   * @brief Удаляет ребро из матрицы смежности
   * @param matrix: матрица смежности
   * @param start_num: начало ребра
   * @param end_num: конец ребра
   * @return AdjacencyMatrix: матрица с удалённым ребром
   */
  AdjacencyMatrix& DeleteEdge_(AdjacencyMatrix& matrix, std::size_t start_num,
                               std::size_t end_num);

  /**
   * @brief Находит место для вставки вершины для соблюдения порядка
   * @param eval: нижняя оценка матрицы
   * @return std::size_t: индекс вставки вершины
   */
  std::size_t FindIndex_(double eval) const;

  /**
   * @brief Замыкает Гамильтонов цикл обхода контрольных точек
   * @param node: вершина графа поиска оптимального пути
   */
  void CompleteEdgePath_(std::shared_ptr<TSPNode> node);

  /**
   * @brief Переводит ребра, содержащихся в пути
   * в последовательность обхода контрольных точек
   * @return std::vector<std::size_t>: последовательность
   * обхода контрольных точек
   */
  std::vector<std::size_t> ConvertToVertexPath_();

  /**
   * @brief Просчитывает оптимальную маршрут
   * @return std::vector<std::size_t>: порядок следования вершин
   */
  std::vector<std::size_t> CalculateTrajectory_();
};

}  // namespace math
