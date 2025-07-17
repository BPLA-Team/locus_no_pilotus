#pragma once

// our code libs:
#include "base.h"

namespace lib {

/// @brief Опасная зона
class TrappyCircle : public JSONable {
 public:
  TrappyCircle(Point center = {0, 0}, double radius = 0);

  TrappyCircle(const TrappyCircle&) = default;
  TrappyCircle(TrappyCircle&&) = default;

  TrappyCircle& operator=(const TrappyCircle&) = default;
  TrappyCircle& operator=(TrappyCircle&&) = default;

  QJsonObject GetJsonInfo() const override;
  void SetJsonInfo(const QJsonObject& trappy_circle_obj) override;

  bool IsChanged(const QJsonObject& trappy_circle_obj) const override;

  Point GetCenter() const { return center_; }
  double GetRadius() const { return radius_; }

  void SetCenter(const Point& center) {
    center_ = center;
    CheckErrorValues_();
  }

  void SetRadius(double r) {
    radius_ = r;
    CheckErrorValues_();
  }

  bool operator==(const TrappyCircle&) const;

 private:
  /**
   * @brief Проверяет данные в классе на валидность
   * @throw std::invalid_argument: если одна из коорд. центра превышает
   * максимально допустимое значение
   * @throw std::invalid_argument: если радиус превышает максимально допустимое
   * значение
   */
  void CheckErrorValues_() const override;

  Point center_;
  double radius_;
};

}  // namespace lib
