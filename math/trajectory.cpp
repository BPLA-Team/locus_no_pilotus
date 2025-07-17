// header file:
#include "trajectory.h"

using lib::Hill;
using lib::Segment;
using lib::Target;
using lib::TrappyCircle;
using lib::TrappyLine;

namespace math {

void TrajectoryCalculator::CalculateTrajectory_() {
  // Матрица смежности контрольных точек
  std::vector<std::vector<double>> matrix(targets_.size());
  for (auto& elem : matrix) elem.resize(targets_.size());

  // Матрица оптимальных путей между контрольными точками
  std::vector<std::vector<std::vector<Segment>>> optimal_ways(targets_.size());
  for (auto& elem : optimal_ways) elem.resize(targets_.size());

  // Заполнение матриц
  OptimalWayCalculator owc(circles_, polys_);

  for (std::size_t i = 0; i < targets_.size(); ++i) {
    matrix[i][i] = inf;
    for (std::size_t j = 0; j < i; ++j) {
      owc.FindOptimalWay(targets_[i], targets_[j]);
      std::vector<Segment> segment_way = owc.GetTrajectoryPart();

      optimal_ways[i][j] = segment_way;
      std::reverse(segment_way.begin(), segment_way.end());
      for (auto& seg_part : segment_way) {
        if (seg_part.IsArc())
          seg_part =
              Segment(seg_part.End(), seg_part.Start(), seg_part.Center());
        else
          seg_part = Segment(seg_part.End(), seg_part.Start());
      }
      optimal_ways[j][i] = segment_way;

      matrix[i][j] = owc.GetOptimalWayLength();
      matrix[j][i] = owc.GetOptimalWayLength();
    }
  }

  for (auto& line : forbidden_lines_) {
    matrix[line.first][line.second] = inf;
    matrix[line.second][line.first] = inf;
  }

  // Просчет кратчайшей траектории по алгоритму Литтла
  AdjacencyMatrix adj_matrix = AdjacencyMatrix::WithExtraRowCol(matrix);
  TravellingSalesmansProblem tsp(adj_matrix, number_of_flyers_);
  std::vector<std::size_t> traj = tsp.GetTrajectory();

  // Объединение частей траектории
  for (std::size_t i = 0; i < traj.size(); ++i) {
    trajectory_.insert(
        trajectory_.end(),
        optimal_ways[traj[i]][traj[(i + 1) % traj.size()]].begin(),
        optimal_ways[traj[i]][traj[(i + 1) % traj.size()]].end());
  }
}

}  // namespace math
