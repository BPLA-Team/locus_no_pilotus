// header file:
#include "plot_area.h"

// our code libs:
#include "math/optimal_way/helpers_functions.h"

namespace data_tools {

bool IsPointInsideHill(lib::Point point, std::vector<lib::Point> vertices) {
  bool inside = false;
  int j = vertices.size() - 1;
  for (size_t i = 0; i < vertices.size(); i++) {
    if (((vertices[i].y < point.y && vertices[j].y >= point.y) ||
         (vertices[j].y < point.y && vertices[i].y >= point.y)) &&
        (vertices[i].x + (point.y - vertices[i].y) /
                             (vertices[j].y - vertices[i].y) *
                             (vertices[j].x - vertices[i].x) <
         point.x))
      inside = !inside;
    j = i;
  }
  return inside;
}

void PlotArea::CheckIntersectionsBetweenTrappyCircles_() {
  if (manager_->GetTrappyCircles().empty()) return;

  for (size_t i = 0; i < manager_->GetTrappyCircles().size() - 1; i++) {
    const auto trc = manager_->GetTrappyCircles()[i];

    for (size_t j = i + 1; j < manager_->GetTrappyCircles().size(); j++) {
      const auto trc2 = manager_->GetTrappyCircles()[j];

      if (lib::DistanceBetweenPoints(trc2.GetCenter(), trc.GetCenter()) <
          trc2.GetRadius() + trc.GetRadius()) {
        std::string text =
            "There are intersections between TrappyCircles! Id's: ";
        text += std::to_string(trc.GetData().GetId());
        text += " and ";
        text += std::to_string(trc2.GetData().GetId());
        throw std::invalid_argument(text);
      }
    }
  }
}

void PlotArea::CheckIntersectionsBetweenHills_() {
  if (manager_->GetHillsPtrs().empty()) return;

  for (size_t i = 0; i < manager_->GetHills().size() - 1; i++) {
    std::vector<math::Point> vec;
    for (const auto& v : manager_->GetHillsPtrs()[i]->GetVertices())
      vec.push_back(math::Point(v));

    for (size_t j = i + 1; j < manager_->GetHillsPtrs().size(); j++) {
      std::vector<math::Point> vec2;
      for (const auto& v : manager_->GetHillsPtrs()[j]->GetVertices())
        vec2.push_back(math::Point(v));

      for (size_t k = 0; k < vec2.size(); k++) {
        if (math::AreThereIntersections(math::PolygonObstacle(vec), vec2[k],
                                        vec2[(k + 1) % vec2.size()]) ||
            IsPointInsideHill(manager_->GetHillsPtrs()[j]->GetVertices()[k],
                              manager_->GetHillsPtrs()[i]->GetVertices())) {
          std::string text = "There are intersections between Hills! Id's: ";
          text +=
              std::to_string(manager_->GetHillsPtrs()[i]->GetData().GetId());
          text += " and ";
          text +=
              std::to_string(manager_->GetHillsPtrs()[j]->GetData().GetId());
          throw std::invalid_argument(text);
        }
      }
    }
  }
}

void PlotArea::CheckTrappyCircles_() {
  // Проверка на пересечения с другими TrappyCircle
  CheckIntersectionsBetweenTrappyCircles_();

  if (manager_->GetTrappyCircles().empty()) return;

  for (const auto& trc : manager_->GetTrappyCircles()) {
    // Проверка на то, что какой-то Target находится внутри TrappyCircle
    for (const auto& target : manager_->GetTargets()) {
      if (lib::DistanceBetweenPoints(target.GetPoint(), trc.GetCenter()) <=
          trc.GetRadius()) {
        std::string text = "There is Target in TrappyCircle! Id's: ";
        text += std::to_string(target.GetData().GetId());
        text += " and ";
        text += std::to_string(trc.GetData().GetId());
        throw std::invalid_argument(text);
      }
    }

    // Проверка на пересечения с Hill
    for (const auto& hill : manager_->GetHillsPtrs()) {
      std::vector<math::Point> vec;
      for (const auto& v : hill->GetVertices()) vec.push_back(math::Point(v));

      for (size_t k = 0; k < vec.size(); k++) {
        if (math::AreThereIntersections(
                math::CircleObstacle(trc.GetCenter(), trc.GetRadius()), vec[k],
                vec[(k + 1) % vec.size()]) ||
            IsPointInsideHill(trc.GetCenter(), hill->GetVertices()) ||
            lib::DistanceBetweenPoints(
                trc.GetCenter(), hill->GetVertices()[k]) < trc.GetRadius()) {
          std::string text =
              "There are intersections between TrappyCircle and Hill! Id's: ";
          text += std::to_string(trc.GetData().GetId());
          text += " and ";
          text += std::to_string(hill->GetData().GetId());
          throw std::invalid_argument(text);
        }
      }
    }
  }
}

void PlotArea::CheckTrappyLines_() {
  auto l = manager_->GetTrappyLinesPtrs().size();
  auto n = manager_->GetTargetsPtrs().size() + amount_of_robots_ - 1;
  if (l == 0) return;

  // прекрасная формула
  if ((pow(n, 2) - n) / 2 - l < n)
    throw std::invalid_argument("There are too many TrappyLines here!");
}

void PlotArea::CheckTargets_() {
  if (manager_->GetTargetsPtrs().size() > 30) {
    std::string text = "There are too many Targets: ";
    text += std::to_string(manager_->GetTargetsPtrs().size());
    text += " > 30 \n";
    text += "(sorry, but our algo is now to slow for more)";
    throw std::invalid_argument(text);
  }
}

void PlotArea::CheckHills_() {
  for (const auto& hill : manager_->GetHills()) {
    // Проверка на пересечения с другими Hill
    CheckIntersectionsBetweenHills_();

    // Проверка на выпуклость многоугольника
    // Определяем знак векторного произведения
    QVector2D v1 = QVector2D(hill.GetVertices()[1].x - hill.GetVertices()[0].x,
                             hill.GetVertices()[1].y - hill.GetVertices()[0].y);
    QVector2D v2 = QVector2D(hill.GetVertices()[2].x - hill.GetVertices()[1].x,
                             hill.GetVertices()[2].y - hill.GetVertices()[1].y);
    short sign = (v1.x() * v2.y() - v1.y() * v2.x()) /
                 abs(v1.x() * v2.y() - v1.y() * v2.x());

    size_t sz = hill.GetVertices().size();
    for (size_t i = 1; i < sz; i++) {
      QVector2D vec1 = QVector2D(
          hill.GetVertices()[(i + 1) % sz].x - hill.GetVertices()[i % sz].x,
          hill.GetVertices()[(i + 1) % sz].y - hill.GetVertices()[i % sz].y);
      QVector2D vec2 = QVector2D(hill.GetVertices()[(i + 2) % sz].x -
                                     hill.GetVertices()[(i + 1) % sz].x,
                                 hill.GetVertices()[(i + 2) % sz].y -
                                     hill.GetVertices()[(i + 1) % sz].y);

      double product = vec1.x() * vec2.y() - vec1.y() * vec2.x();

      if (product == 0 || product / abs(product) != sign) {
        std::string text = "There is non-convex polygon! Id: ";
        text += std::to_string(hill.GetData().GetId());
        throw std::invalid_argument(text);
      }
    }

    // Проверка на то, что какой-то Target находится внутри или на Hill
    for (const auto& target : manager_->GetTargets())
      if (IsPointInsideHill(target.GetPoint(), hill.GetVertices())) {
        std::string text = "There is Target in Hill! Id's: ";
        text += std::to_string(target.GetData().GetId());
        text += " and ";
        text += std::to_string(hill.GetData().GetId());
        throw std::invalid_argument(text);
      }
  }
}

}  // namespace data_tools
