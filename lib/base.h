#pragma once

// Qt libs:
#include <QJsonArray>
#include <QJsonObject>

// our code libs:
#include "point.h"  // used everywhere with lib

namespace lib {

constexpr double max_coord = 1000000;

/// @brief Объект, возможный к считыванию по .json файлу [абстрактный класс]
class JSONable {
 public:
  virtual QJsonObject GetJsonInfo() const = 0;
  virtual void SetJsonInfo(const QJsonObject& obj) = 0;

  virtual bool IsChanged(const QJsonObject& obj) const = 0;

  /**
   * @brief Возвращает значение ид. объекта внутри файла
   * @return unsigned short: идентификатор объекта внутри файла
   */
  virtual unsigned short GetId() const { return id_; }

  /**
   * @brief Устанавливает значение ид. объекта внутри файла
   * @param id: новый идентификатор объекта
   */
  virtual void SetId(unsigned short id) { id_ = id; }

 protected:
  /// @brief Проверяет данные в классе на валидность
  virtual void CheckErrorValues_() const = 0;

 private:
  /// @brief Идентификатор объекта внутри файла
  unsigned short id_{USHRT_MAX};
};

}  // namespace lib
