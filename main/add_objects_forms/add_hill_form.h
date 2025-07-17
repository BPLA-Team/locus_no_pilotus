#pragma once

// Qt libs:
#include <QDialog>
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>

// std libs:
#include <memory>
#include <vector>

// our code libs:
#include "lib/point.h"

namespace Ui {
class AddHillForm;
}

/// @brief Структура из двух QLineEdit, относящихся к вводу координаты точки
/// (чтобы не использовать std::pair<std::unique_ptr<QLineEdit>, ... >)
struct PointLineEdits {
  PointLineEdits() = default;
  PointLineEdits(QLineEdit* abscissa, QLineEdit* ordinate)
      : abscissa{abscissa}, ordinate{ordinate} {}

  std::unique_ptr<QLineEdit> abscissa;
  std::unique_ptr<QLineEdit> ordinate;
};

class AddHillForm : public QDialog {
  Q_OBJECT

 public:
  explicit AddHillForm(QWidget* parent = nullptr);
  ~AddHillForm();

 signals:
  void AddHill(const std::vector<std::pair<double, double>>& points);

 private slots:
  void on_createPushButton_clicked();
  void on_clearPushButton_clicked();
  void on_newPushButton_clicked();
  void on_deletePushButton_clicked();

 private:
  /**
   * @brief Добавляет определенное количество новых полей ввода на форму
   * @param amount: кол-ло новых полей ввода точки
   */
  void AddNewInputFields_(size_t amount = 1);

  Ui::AddHillForm* ui;
  std::vector<std::unique_ptr<QWidget>> point_layouts_widgets_;
  std::vector<PointLineEdits> both_coords_point_line_edits_;
};
