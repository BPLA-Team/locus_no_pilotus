#pragma once
#include "trajectory.h"

enum class SpeedOfRobot { Low, Medium, High };

namespace gui {

/**
 * @brief Класс, который позволяет анимировать движение робота
 * @details Представляет из себя png картинку,
 * которая движется по траектории
 */
class FlyingRobot : public Drawable {
 public:
  FlyingRobot() = default;

  /**
   * @brief Инициализирует новый экземпляр FlyingRobot
   * @param gui::Trajectory: траектория
   */
  FlyingRobot(gui::Trajectory* trj)
      : trajectory_{trj}, curr_point_{trj->Segments()[0].Start()} {
    // Сразу задаем нужные приватные поля при инициализации экземпляра класса, в
    // зависимости от того, lib::Segment является аркой или прямой линией
    if (trajectory_->Segments()[0].IsArc())
      UpdateCircleFields_();
    else
      UpdateLineFields_();
  }

  /**
   * @brief Отрисовывает png картинку
   * @param plot: указатель на полотно
   */
  void Draw(QCustomPlot* plot) override;

  /**
   * @brief Эта функция нужна для того, чтобы обновлять позицию картинки на
   * полотне
   * @param plot: указатель на полотно
   */
  void ReDraw(QCustomPlot* plot);

  void SetTrajectory(gui::Trajectory* trj);

  gui::Trajectory* GetTrajectory() const { return trajectory_; }

  void SetSpeed(SpeedOfRobot speed) { speed_ = speed; }

 private:
  gui::Trajectory* trajectory_;
  QCPGraph* graph_{nullptr};
  SpeedOfRobot speed_ = SpeedOfRobot::Medium;

  size_t index_of_segment_ = 0;
  lib::Point curr_point_;

  // Количество отрезков, на которые будет делиться lib::Segment
  int count_of_partitions_;

  // Поля для положения на линии
  double line_const_;
  double cos_of_line_;
  double sin_of_line_;

  // Поля для положения на окружности

  double curr_angle_on_circle_;
  // Величина смещения угла
  double distribution_of_angle_;
  // Если true, что двигаемся по часовой
  bool clockwise_;

  /**
   * @brief Обновляет поля cos_of_line_ и sin_of_line_
   */
  void UpdateLineFields_();

  /**
   * @brief Обновляет поля curr_angle_on_circle_, clockwise_ и
   * distribution_of_angle_
   */
  void UpdateCircleFields_();

  /**
   * @brief Берет следующий Segment, обновляя приватные поля
   */
  void UpdateSegment_();

  /**
   * @brief Обновляет текущее положение точки на линии
   */
  void SetNewPositionOnLine_();

  /**
   * @brief Обновляет текущее положение точки на окружности
   */
  void SetNewPositionOnCircle_();
};

}  // namespace gui
