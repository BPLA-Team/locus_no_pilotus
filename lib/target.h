#pragma once

// our code libs
#include "base.h"

namespace lib {

/**
 * @brief Контрольная точка
 * @details Самый лучший класс в мире: только с ним у нас не было проблем :)
 */
class Target : public JSONable {
 public:
  Target(double x, double y) : p_(x, y) { CheckErrorValues_(); }
  Target(const Point& p) : p_{p} { CheckErrorValues_(); }
  Target() : p_(0, 0) {}

  Target(const Target&) = default;
  Target(Target&&) = default;

  Target& operator=(const Target&) = default;
  Target& operator=(Target&&) = default;

  QJsonObject GetJsonInfo() const override;
  void SetJsonInfo(const QJsonObject& target_obj) override;

  bool IsChanged(const QJsonObject& target_obj) const override;

  Point GetPoint() const { return p_; }

  void SetPoint(const Point& p) {
    p_ = p;
    CheckErrorValues_();
  }

  void SetPoint(double x, double y) {
    p_ = Point(x, y);
    CheckErrorValues_();
  }

  bool operator==(const Target&) const;

 private:
  /**
   * @brief Проверяет данные в классе на валидность
   * @throw std::invalid_argument: если одна из коорд. точки превышает
   * максимально допустимое значение
   */
  void CheckErrorValues_() const override;

  Point p_;
};

}  // namespace lib
