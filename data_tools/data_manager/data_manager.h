#pragma once

// our code libs:
#include "gui/_objects.h"

namespace data_tools {

/// @brief Класс, хранящий gui объекты и связывающий их с QCustomPlot
class DataManager {
 public:
  DataManager() = default;

  /**
   * @brief Удаляет объект из менеджера по индексу
   * @param obj_type: тип объекта
   * @param index: индекс объекта в его векторе
   */
  void Remove(gui::ObjectType obj_type, size_t index);

  /// @brief Очищает все вектора объектов
  void Clear();

  // ----------------------   Target methods   ----------------------

  // for gui::Target
  void Add(gui::Target* target_ptr);
  void Add(std::vector<gui::Target*>);
  void Set(std::vector<gui::Target*>);

  // for lib::Target
  void Add(lib::Target& data);
  void Add(const lib::Target& data);

  void Add(std::vector<lib::Target>&);
  void Add(const std::vector<lib::Target>&);

  void Set(std::vector<lib::Target>&);
  void Set(const std::vector<lib::Target>&);

  /**
   * @brief Возвращает значение Targets
   * @return std::vector<gui::Target*>: указатели на объекты к.т.
   */
  std::vector<gui::Target*> GetTargetsPtrs();

  /**
   * @brief Возвращает значение Targets
   * @return std::vector<gui::Target>: объекты к.т.
   */
  std::vector<gui::Target> GetTargets() const;

  // ----------------------    Hill methods    ----------------------

  // for gui::Hill
  void Add(gui::Hill* hill_ptr);
  void Add(std::vector<gui::Hill*>);
  void Set(std::vector<gui::Hill*>);

  // for lib::Hill
  void Add(lib::Hill& data);
  void Add(const lib::Hill& data);

  void Add(std::vector<lib::Hill>&);
  void Add(const std::vector<lib::Hill>&);

  void Set(std::vector<lib::Hill>&);
  void Set(const std::vector<lib::Hill>&);

  /**
   * @brief Возвращает значение Hills
   * @return std::vector<gui::Hill*>: указатели на объекты рельефа
   */
  std::vector<gui::Hill*> GetHillsPtrs();

  /**
   * @brief Возвращает значение Hills
   * @return std::vector<gui::Hill>: объекты рельефа
   */
  std::vector<gui::Hill> GetHills() const;

  // ---------------------- TrappyCircle methods ----------------------

  // for gui::TrappyCircle
  void Add(gui::TrappyCircle* trappy_circle);
  void Add(std::vector<gui::TrappyCircle*>);
  void Set(std::vector<gui::TrappyCircle*>);

  // for lib::TrappyCircle
  void Add(lib::TrappyCircle& data);
  void Add(const lib::TrappyCircle& data);

  void Add(std::vector<lib::TrappyCircle>&);
  void Add(const std::vector<lib::TrappyCircle>&);

  void Set(std::vector<lib::TrappyCircle>&);
  void Set(const std::vector<lib::TrappyCircle>&);

  /**
   * @brief Возвращает значение TrappyCircles
   * @return std::vector<gui::TrappyCircle*>: указатели на объекты опасной зоны
   */
  std::vector<gui::TrappyCircle*> GetTrappyCirclesPtrs();

  /**
   * @brief Возвращает значение TrappyCircles
   * @return std::vector<gui::TrappyCircle>: объекты опасной зоны
   */
  std::vector<gui::TrappyCircle> GetTrappyCircles() const;

  // ----------------------  TrappyLine methods  ----------------------

  // for gui::TrappyLine
  void Add(gui::TrappyLine* trappy_line);
  void Add(std::vector<gui::TrappyLine*>);
  void Set(std::vector<gui::TrappyLine*>);

  // for lib::TrappyLine
  void Add(lib::TrappyLine& data);
  void Add(const lib::TrappyLine& data);

  void Add(std::vector<lib::TrappyLine>&);
  void Add(const std::vector<lib::TrappyLine>&);

  void Set(std::vector<lib::TrappyLine>&);
  void Set(const std::vector<lib::TrappyLine>&);

  /**
   * @brief Возвращает значение TrappyLines
   * @return std::vector<gui::TrappyLine*>: указатели на объекты оп. перелета
   */
  std::vector<gui::TrappyLine*> GetTrappyLinesPtrs();

  /**
   * @brief Возвращает значение TrappyLines
   * @return std::vector<gui::TrappyLine>: объекты оп. перелета
   */
  std::vector<gui::TrappyLine> GetTrappyLines() const;

  /**
   * @brief Удаляет последний объект в векторах, если он является дупликатом
   * другого
   * @return true: если был удалён хоть один объект
   * @return false: если не был
   */
  bool RemoveLastDuplicate();

  /**
   * @brief Удаляет все повторяющиеся объекты в векторах объектов
   * @return true: если был удалён хоть один объект
   * @return false: если не был
   */
  bool RemoveAllDuplicates();

 private:
  /**
   * @brief Проверяет данные в DataManager на валидность
   * @throw std::invalid_argument: если объектов какого-либо вектора > 10000
   */
  void CheckErrorValues_();

  unsigned short GetMinId_(gui::ObjectType obj_type);

  std::vector<std::unique_ptr<gui::Hill>> hills_;
  std::vector<std::unique_ptr<gui::Target>> targets_;
  std::vector<std::unique_ptr<gui::TrappyCircle>> trappy_circles_;
  std::vector<std::unique_ptr<gui::TrappyLine>> trappy_lines_;
};

}  // namespace data_tools
