#pragma once

// our code libs:
#include "base.h"
#include "lib/trappy_circle.h"

namespace gui {

/**
 * @brief Фигура опасной зоны
 * @details Фигура представляет собой красный круг с полупрозрачной заливкой
 * (красный по умолчанию, однако цвет задаётся в аргументах конструктора)
 */
class TrappyCircle : public Drawable {
 public:
  TrappyCircle() = default;

  TrappyCircle(double x, double y, double radius,
               QColor color = QColor(200, 50, 50, 255))
      : data_(lib::Point{x, y}, radius), color_{color} {}

  TrappyCircle(lib::Point center, double radius,
               QColor color = QColor(200, 50, 50, 255))
      : data_(center, radius), color_{color} {}

  TrappyCircle(const lib::TrappyCircle& data,
               QColor color = QColor(200, 50, 50, 255))
      : data_(data), color_{color} {}

  TrappyCircle(const TrappyCircle&) = default;
  TrappyCircle(TrappyCircle&&) = default;

  TrappyCircle& operator=(const TrappyCircle&) = default;
  TrappyCircle& operator=(TrappyCircle&&) = default;

  lib::Point GetCenter() const { return data_.GetCenter(); }
  double GetRadius() const { return data_.GetRadius(); }
  QColor GetColor() const { return color_; }
  const lib::TrappyCircle& GetData() const { return data_; }
  lib::TrappyCircle& GetData() { return data_; }

  void SetCenter(const lib::Point& center) { data_.SetCenter(center); }
  void SetRadius(double radius) { data_.SetRadius(radius); }

  /**
   * @brief Отрисовывает фигуру на полотне
   * @details Фигура представляет собой красный круг с полупрозрачной заливкой
   * (красный по умолчанию, однако цвет задаётся в аргументах конструктора)
   * @param plot: указатель на полотно
   */
  void Draw(QCustomPlot* plot) override;

  /**
   * @brief Возвращает значение указателя на полотне
   * @return QCPItemEllipse*: указатель
   */
  QCPItemEllipse* GetItemEllipsePtr() const { return ellipse_; }

  bool operator==(const gui::TrappyCircle& tr_circle) const {
    return data_ == tr_circle.GetData();
  }

 private:
  lib::TrappyCircle data_;
  QColor color_;

  QCPItemEllipse* ellipse_{nullptr};
};

}  // namespace gui
