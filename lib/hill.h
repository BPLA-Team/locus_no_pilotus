#pragma once

// std libs:
#include <initializer_list>
#include <vector>

// our code libs:
#include "base.h"

namespace lib {

/// @brief Рельеф с высотой
class Hill : public JSONable {
 public:
  Hill() = default;

  /**
   * @brief Инициализирует новый экземпляр Hill
   * (так как рельеф является многоугольником, его можно построить по точкам)
   * @param points: список точек
   */
  Hill(const std::vector<Point>& points) : vertices_{points} {
    CheckErrorValues_();
  }

  Hill(const Hill&) = default;
  Hill(Hill&&) = default;

  Hill& operator=(const Hill&) = default;
  Hill& operator=(Hill&&) = default;

  QJsonObject GetJsonInfo() const override;
  void SetJsonInfo(const QJsonObject& hill_obj) override;

  bool IsChanged(const QJsonObject& hill_obj) const override;

  const std::vector<Point>& GetVertices() const { return vertices_; }
  std::vector<Point>& GetVertices() { return vertices_; }

  void AddVertice(Point vertice) { vertices_.push_back(vertice); }

  bool operator==(const Hill&) const;

 private:
  /**
   * @brief Проверяет данные в классе на валидность
   * @throw std::invalid_argument: если одна из коорд. вершин превышает
   * максимально допустимое значение
   */
  void CheckErrorValues_() const override;

  std::vector<Point> vertices_;
};

}  // namespace lib
