#pragma once

// Qt libs:
#include <QTableWidget>

// our code libs:
#include "data_tools/plot_area/plot_area.h"

namespace data_tools {

/// @brief Класс, упрощающий отображение классов gui в QTableWidget's
class TablesConnection : public QObject {
 public:
  TablesConnection() = default;

  void Setup(DataManager* manager, PlotArea* area);

  /**
   * @brief Устанавливает значения всех таблиц с информацией
   * @param hills_info: QTableWidget с информацией о hills
   * @param targets_info: QTableWidget с информацией о hills
   * @param tr_circles_info: QTableWidget с информацией о hills
   * @param tr_lines_info: QTableWidget с информацией о hills
   */
  void SetSettingsTables(QTableWidget* targets_info, QTableWidget* hills_info,
                         QTableWidget* tr_circles_info,
                         QTableWidget* tr_lines_info) {
    targets_table_.reset(targets_info);
    hills_table_.reset(hills_info);
    tr_circles_table_.reset(tr_circles_info);
    tr_lines_table_.reset(tr_lines_info);
    UpdateTablesConnections_();
  }

  /**
   * @brief Устанавливает значение всех кнопок для удаления объектов
   * @param targets_remove_button
   * @param hills_remove_button
   * @param tr_circles_remove_button
   * @param tr_lines_remove_button
   */
  void SetRemoveButtons(QPushButton* targets_remove_button,
                        QPushButton* hills_remove_button,
                        QPushButton* tr_circles_remove_button,
                        QPushButton* tr_lines_remove_button) {
    targets_remove_button_.reset(targets_remove_button);
    hills_remove_button_.reset(hills_remove_button);
    tr_circles_remove_button_.reset(tr_circles_remove_button);
    tr_lines_remove_button_.reset(tr_lines_remove_button);
    UpdateRemoveButtonConnections_();
  }

  /// @brief Обновляет данные в таблицах всех объектов
  void UpdateTables();

  /**
   * @brief Обновляет значения таблицы с определенным типом объекта
   * @param obj_type: тип объекта
   */
  void UpdateTable(gui::ObjectType obj_type);

 private slots:
  void TargetsItemChanged(int row, int column);
  void HillsItemChanged(int row, int column);
  void TrappyCirclesItemChanged(int row, int column);
  void TrappyLinesItemChanged(int row, int column);

  void RemoveTargetItem();
  void RemoveHillItem();
  void RemoveTrappyCircleItem();
  void RemoveTrappyLineItem();

  void EnableRemoveTargetButton(int row, int column) {
    Q_UNUSED(row);
    selected_column_ = column;
    targets_remove_button_->setEnabled(true);
  }

  void EnableRemoveHillButton(int row, int column) {
    Q_UNUSED(row);
    selected_column_ = column;
    hills_remove_button_->setEnabled(true);
  }

  void EnableRemoveTrappyCircleButton(int row, int column) {
    Q_UNUSED(row);
    selected_column_ = column;
    tr_circles_remove_button_->setEnabled(true);
  }

  void EnableRemoveTrappyLineButton(int row, int column) {
    Q_UNUSED(row);
    selected_column_ = column;
    tr_lines_remove_button_->setEnabled(true);
  }

  void DisableRemoveTargetButton() {
    targets_remove_button_->setEnabled(false);
  }

  void DisableRemoveHillButton() { hills_remove_button_->setEnabled(false); }

  void DisableRemoveTrappyCircleButton() {
    tr_circles_remove_button_->setEnabled(false);
  }

  void DisableRemoveTrappyLineButton() {
    tr_lines_remove_button_->setEnabled(false);
  }

 private:
  /**
   * @brief Обновляет значения таблицы с Targets
   * @param targets: вектор новых значений
   */
  void UpdateTable_(const std::vector<gui::Target>& targets);

  /**
   * @brief Обновляет значения таблицы с Hills
   * @param hills: вектор новых значений
   */
  void UpdateTable_(const std::vector<gui::Hill>& hills);

  /**
   * @brief Обновляет значения таблицы с TrappyLines
   * @param trappy_lines: вектор новых значений
   */
  void UpdateTable_(const std::vector<gui::TrappyLine>& trappy_lines);

  /**
   * @brief Обновляет значения таблицы с TrappyCircles
   * @param trappy_circles: вектор новых значений
   */
  void UpdateTable_(const std::vector<gui::TrappyCircle>& trappy_circles);

  int selected_column_{INT_MAX};

  void UpdateTablesConnections_();
  void DisableTablesConnections_();
  void UpdateRemoveButtonConnections_();

  std::unique_ptr<QTableWidget> targets_table_{nullptr};
  std::unique_ptr<QTableWidget> hills_table_{nullptr};
  std::unique_ptr<QTableWidget> tr_circles_table_{nullptr};
  std::unique_ptr<QTableWidget> tr_lines_table_{nullptr};

  std::unique_ptr<QPushButton> targets_remove_button_{nullptr};
  std::unique_ptr<QPushButton> hills_remove_button_{nullptr};
  std::unique_ptr<QPushButton> tr_circles_remove_button_{nullptr};
  std::unique_ptr<QPushButton> tr_lines_remove_button_{nullptr};

  std::unique_ptr<DataManager> manager_;
  std::unique_ptr<PlotArea> area_;
};

}  // namespace data_tools
