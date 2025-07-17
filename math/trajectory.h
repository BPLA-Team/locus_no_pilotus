#pragma once

// our code libs:
#include "lib/_objects.h"
#include "math/littles_algorithm/travelling_salesmans_problem.h"
#include "math/optimal_way/optimal_way.h"

namespace math {

class TrajectoryCalculator {
 public:
  /**
   * @brief Инициализирует экземпляр класса TrajectoryCalculator
   * @param targets: контрольные точки
   * @param lines: запрещённые перелеты
   * @param circles: круговые препятствия
   * @param hills: многоугольные препятствия
   */
  TrajectoryCalculator(const std::vector<lib::Target>& targets,
                       const std::vector<lib::TrappyLine>& lines,
                       const std::vector<lib::TrappyCircle>& circles,
                       const std::vector<lib::Hill>& hills,
                       unsigned short num_of_flyers)
      : number_of_flyers_{num_of_flyers} {
    for (auto& target : targets) targets_.push_back(Point(target.GetPoint()));

    for (auto& line : lines) {
      std::pair<std::size_t, std::size_t> indexes;
      std::pair<lib::Target, lib::Target> points = line.GetTargets();
      for (std::size_t i = 0; i < targets.size(); ++i)
        if (targets[i] == points.first)
          indexes.first = i;
        else if (targets[i] == points.second)
          indexes.second = i;
      forbidden_lines_.push_back(indexes);
    }

    for (auto& circle : circles)
      circles_.push_back(
          CircleObstacle(Point(circle.GetCenter()), circle.GetRadius()));

    for (auto& hill : hills) {
      std::vector<Point> vertexes;
      for (auto& vertex : hill.GetVertices()) vertexes.push_back(Point(vertex));
      polys_.push_back(PolygonObstacle(vertexes));
    }

    CalculateTrajectory_();
  }

  /// @brief Возвращает траекторию
  std::vector<lib::Segment> GetTrajectory() { return trajectory_; }

 private:
  // Количество роботов
  unsigned short number_of_flyers_;

  // Контрольные точки
  std::vector<Point> targets_;

  // Запрещенные перелёты
  std::vector<std::pair<std::size_t, std::size_t>> forbidden_lines_;

  // Круговые препятствия
  std::vector<CircleObstacle> circles_;

  // Многоугольные препятствия
  std::vector<PolygonObstacle> polys_;

  // Траектория облета контрольных точек
  std::vector<lib::Segment> trajectory_;

  /// @brief Рассчитывает траекторию
  void CalculateTrajectory_();
};

}  // namespace math
