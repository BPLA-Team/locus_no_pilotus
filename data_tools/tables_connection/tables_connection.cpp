// header file:
#include "tables_connection.h"

namespace data_tools {

void TablesConnection::Setup(DataManager* manager, PlotArea* area) {
  manager_.reset(manager);
  area_.reset(area);
}

// MARK: U.T. by Targets

void TablesConnection::UpdateTable_(const std::vector<gui::Target>& targets) {
  DisableTablesConnections_();

  if (targets.empty()) {
    targets_table_->setColumnCount(0);
    return;
  }

  // кол-во столбцов = кол-во к.т.
  targets_table_->setColumnCount(static_cast<int>(targets.size()));

  for (size_t i = 0; i < targets.size(); i++) {
    // устанавливаем столбик с номером объекта
    auto item = new QTableWidgetItem("№ " + QString::number(i + 1));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    targets_table_->setHorizontalHeaderItem(static_cast<int>(i),
                                            item);  // номер к.т. = индекс + 1

    // в строки добавляем индекс на полотне и координаты
    item = new QTableWidgetItem(QString::number(targets[i].GetData().GetId()));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    targets_table_->setItem(0, static_cast<int>(i), item);

    item = new QTableWidgetItem(QString::number(targets[i].GetPoint().x));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |
                   Qt::ItemIsEnabled);
    targets_table_->setItem(1, static_cast<int>(i), item);

    item = new QTableWidgetItem(QString::number(targets[i].GetPoint().y));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |
                   Qt::ItemIsEnabled);
    targets_table_->setItem(2, static_cast<int>(i), item);
  }

  targets_table_->update();
  UpdateTablesConnections_();
}

// MARK: U.T. by Hills

void TablesConnection::UpdateTable_(const std::vector<gui::Hill>& hills) {
  DisableTablesConnections_();

  if (hills.empty()) {
    hills_table_->setColumnCount(0);
    return;
  }

  // находим максимально возможное кол-во точек среди всех холмов
  size_t hills_max_points = hills[0].GetVertices().size();
  for (size_t i = 0; i < hills.size() - 1; i++) {
    hills_max_points = std::max(hills[i].GetVertices().size(),
                                hills[i + 1].GetVertices().size());
  }

  // кол-во столбцов = кол-во рельефов
  hills_table_->setColumnCount(static_cast<int>(hills.size()));

  // кол-во строк = максимальное число точек * 2 (т.к. x и y) и индекс
  hills_table_->setRowCount(static_cast<int>(hills_max_points * 2 + 1));

  for (size_t i = 0; i < hills.size(); i++) {
    // устанавливаем столбик с номером объекта
    auto item = new QTableWidgetItem("№ " + QString::number(i + 1));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    hills_table_->setHorizontalHeaderItem(static_cast<int>(i),
                                          item);  // номер рельефа = индекс + 1

    // в строки добавляем индекс на полотне и координаты всех точек
    item = new QTableWidgetItem(QString::number(hills[i].GetData().GetId()));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    hills_table_->setItem(0, static_cast<int>(i), item);

    for (size_t j = 1; j < hills[i].GetVertices().size() * 2; j += 2) {
      // названия строк с координатами
      hills_table_->setVerticalHeaderItem(
          static_cast<int>(j),
          new QTableWidgetItem("X_" + QString::number(j / 2 + 1)));
      hills_table_->setVerticalHeaderItem(
          static_cast<int>(j + 1),
          new QTableWidgetItem("Y_" + QString::number(j / 2 + 1)));

      // сами координаты
      item = new QTableWidgetItem(
          QString::number(hills[i].GetVertices()[(j - 1) / 2].x));
      item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |
                     Qt::ItemIsEnabled);
      hills_table_->setItem(static_cast<int>(j), static_cast<int>(i), item);

      item = new QTableWidgetItem(
          QString::number(hills[i].GetVertices()[(j - 1) / 2].y));
      item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |
                     Qt::ItemIsEnabled);
      hills_table_->setItem(static_cast<int>(j + 1), static_cast<int>(i), item);
    }
  }

  hills_table_->update();
  UpdateTablesConnections_();
}

// MARK: U.T. by Tr. Lines

void TablesConnection::UpdateTable_(
    const std::vector<gui::TrappyLine>& trappy_lines) {
  DisableTablesConnections_();

  if (trappy_lines.empty()) {
    tr_lines_table_->setColumnCount(0);
    return;
  }

  // кол-во столбцов = кол-во опасных линий
  tr_lines_table_->setColumnCount(static_cast<int>(trappy_lines.size()));

  for (size_t i = 0; i < trappy_lines.size(); i++) {
    // устанавливаем столбик с номером объекта
    auto item = new QTableWidgetItem("№ " + QString::number(i + 1));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    tr_lines_table_->setHorizontalHeaderItem(static_cast<int>(i),
                                             item);  // номер линии = индекс + 1

    // в строки добавляем индекс на полотне
    item = new QTableWidgetItem(
        QString::number(trappy_lines[i].GetData().GetId()));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    tr_lines_table_->setItem(0, static_cast<int>(i), item);

    // нахождение номеров двух привязанных к.т. в векторе менеджера
    size_t t_1_n{ULLONG_MAX};
    size_t t_2_n{ULLONG_MAX};

    for (size_t j = 0; j < manager_->GetTargets().size(); j++) {
      auto target_ptr = manager_->GetTargetsPtrs()[j];

      if (target_ptr == trappy_lines[i].GetTargetsPtrs().first)
        t_1_n = j + 1;  // номер  = индекс + 1
      if (target_ptr == trappy_lines[i].GetTargetsPtrs().second)
        t_2_n = j + 1;  // номер  = индекс + 1
    }

    item = new QTableWidgetItem(QString::number(t_1_n));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |
                   Qt::ItemIsEnabled);
    tr_lines_table_->setItem(1, static_cast<int>(i), item);

    item = new QTableWidgetItem(QString::number(t_2_n));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |
                   Qt::ItemIsEnabled);
    tr_lines_table_->setItem(2, static_cast<int>(i), item);

    // если эти номера остались огромными, то валидные к.т. не нашлись
    if (t_1_n == ULLONG_MAX || t_2_n == ULLONG_MAX) {
      QMessageBox::warning(tr_lines_table_.get(), "Error!",
                           "Wrong targets numbers in TrappyLines!");

      manager_->Remove(gui::ObjectType::TrappyLines, i);
      tr_lines_table_->removeColumn(i);
      area_->ReDraw();
    }
  }

  tr_lines_table_->update();
  UpdateTablesConnections_();
}

// MARK: U.T. by Tr. Circles

void TablesConnection::UpdateTable_(
    const std::vector<gui::TrappyCircle>& trappy_circles) {
  DisableTablesConnections_();

  if (trappy_circles.empty()) {
    tr_circles_table_->setColumnCount(0);
    return;
  }

  // кол-во столбцов = кол-во опасных зон
  tr_circles_table_->setColumnCount(static_cast<int>(trappy_circles.size()));

  for (size_t i = 0; i < trappy_circles.size(); i++) {
    // устанавливаем столбик с номером объекта
    auto item = new QTableWidgetItem("№ " + QString::number(i + 1));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    tr_circles_table_->setHorizontalHeaderItem(
        static_cast<int>(i), item);  // номер зоны = индекс + 1

    // в строки добавляем индекс на полотне, координаты точки и радиус
    item = new QTableWidgetItem(
        QString::number(trappy_circles[i].GetData().GetId()));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    tr_circles_table_->setItem(0, static_cast<int>(i), item);

    item =
        new QTableWidgetItem(QString::number(trappy_circles[i].GetCenter().x));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |
                   Qt::ItemIsEnabled);
    tr_circles_table_->setItem(1, static_cast<int>(i), item);

    item =
        new QTableWidgetItem(QString::number(trappy_circles[i].GetCenter().y));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |
                   Qt::ItemIsEnabled);
    tr_circles_table_->setItem(2, static_cast<int>(i), item);

    item = new QTableWidgetItem(QString::number(trappy_circles[i].GetRadius()));
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable |
                   Qt::ItemIsEnabled);
    tr_circles_table_->setItem(3, static_cast<int>(i), item);
  }

  tr_circles_table_->update();
  UpdateTablesConnections_();
}

void TablesConnection::UpdateTable(gui::ObjectType obj_type) {
  switch (obj_type) {
    case gui::ObjectType::Targets:
      UpdateTable_(manager_->GetTargets());
      break;

    case gui::ObjectType::Hills:
      UpdateTable_(manager_->GetHills());
      break;

    case gui::ObjectType::TrappyCircles:
      UpdateTable_(manager_->GetTrappyCircles());
      break;

    case gui::ObjectType::TrappyLines:
      UpdateTable_(manager_->GetTrappyLines());
      break;
  }
}

void TablesConnection::UpdateTables() {
  UpdateTable(gui::ObjectType::Targets);
  UpdateTable(gui::ObjectType::Hills);
  UpdateTable(gui::ObjectType::TrappyCircles);
  UpdateTable(gui::ObjectType::TrappyLines);

  UpdateTablesConnections_();
}

// TODO: переписать так, чтобы оно меняло конкретное поле, а не целую точку

// MARK: Targets Item C.

void TablesConnection::TargetsItemChanged(int row, int column) {
  Q_UNUSED(row);

  auto x_item = targets_table_->item(1, column);
  auto y_item = targets_table_->item(2, column);

  try {
    if (x_item != nullptr && y_item != nullptr &&
        static_cast<size_t>(column) < manager_->GetTargets().size()) {
      manager_->GetTargetsPtrs()[column]->SetPoint(x_item->text().toDouble(),
                                                   y_item->text().toDouble());

      area_->ReDraw();
    }

  } catch (const std::exception& e) {
    QMessageBox::critical(targets_table_.get(), "Error!", e.what());

    manager_->Remove(gui::ObjectType::Targets, static_cast<size_t>(column));
    UpdateTable(gui::ObjectType::Targets);

    // (в случае удаления к.т., которая была привязана, надо обновить)
    UpdateTable(gui::ObjectType::TrappyLines);
    area_->ReDraw();
  }
}

// MARK: Hills Item C.

void TablesConnection::HillsItemChanged(int row, int column) {
  auto x_item = hills_table_->item(row, column);
  auto y_item = hills_table_->item(row, column);

  size_t p_index{ULLONG_MAX};

  // здесь важно понимать, что из-за айдишника нумерация сдвинута
  // поэтому X на нечетных, а Y на четных
  bool is_y_item_changed = row % 2 == 0;

  if (is_y_item_changed) {
    // если попали в Y, то предыдущий X (той же точки)
    x_item = hills_table_->item(row - 1, column);
    p_index = row / 2 - 1;
  } else {
    // если попали в X, то следующий Y (той же точки)
    y_item = hills_table_->item(row + 1, column);
    p_index = (row - 1) / 2;
  }

  try {
    if (x_item != nullptr && y_item != nullptr &&
        static_cast<size_t>(column) < manager_->GetHills().size() &&
        p_index < manager_->GetHills()[column].GetVertices().size()) {
      manager_->GetHillsPtrs()[column]->GetVertices()[p_index].x =
          x_item->text().toDouble();
      manager_->GetHillsPtrs()[column]->GetVertices()[p_index].y =
          y_item->text().toDouble();

      area_->ReDraw();
    }

  } catch (const std::exception& e) {
    QMessageBox::critical(targets_table_.get(), "Error!", e.what());

    manager_->Remove(gui::ObjectType::Hills, static_cast<size_t>(column));
    UpdateTable(gui::ObjectType::Hills);
    area_->ReDraw();
  }
}

// MARK: Tr. Circles Item C.

void TablesConnection::TrappyCirclesItemChanged(int row, int column) {
  Q_UNUSED(row);

  auto x_item = tr_circles_table_->item(1, column);
  auto y_item = tr_circles_table_->item(2, column);
  auto r_item = tr_circles_table_->item(3, column);

  try {
    if (x_item != nullptr && y_item != nullptr && r_item != nullptr &&
        static_cast<size_t>(column) < manager_->GetTrappyCircles().size()) {
      manager_->GetTrappyCirclesPtrs()[column]->SetCenter(
          {x_item->text().toDouble(), y_item->text().toDouble()});
      manager_->GetTrappyCirclesPtrs()[column]->SetRadius(
          r_item->text().toDouble());

      area_->ReDraw();
    }

  } catch (const std::exception& e) {
    QMessageBox::critical(targets_table_.get(), "Error!", e.what());

    manager_->Remove(gui::ObjectType::TrappyCircles,
                     static_cast<size_t>(column));
    UpdateTable(gui::ObjectType::TrappyCircles);
    area_->ReDraw();
  }
}

// MARK: Tr. Lines Item C.

void TablesConnection::TrappyLinesItemChanged(int row, int column) {
  Q_UNUSED(row);

  auto t_1_item = tr_lines_table_->item(1, column);
  auto t_2_item = tr_lines_table_->item(2, column);

  try {
    if (t_1_item != nullptr && t_2_item != nullptr &&
        static_cast<size_t>(column) < manager_->GetTrappyLines().size()) {
      // мы в клетках таблицы храним номера, так что для индекса нужно вычесть 1
      auto t_1_index = t_1_item->text().toULong() - 1;
      auto t_2_index = t_2_item->text().toULong() - 1;

      if (t_1_index < manager_->GetTargets().size() &&
          t_2_index < manager_->GetTargets().size()) {
        manager_->GetTrappyLinesPtrs()[column]->SetTargets(
            manager_->GetTargetsPtrs()[t_1_index],
            manager_->GetTargetsPtrs()[t_2_index]);

        area_->ReDraw();

      } else
        throw std::invalid_argument("Wrong targets numbers in TrappyLines!");
    }

  } catch (const std::exception& e) {
    QMessageBox::warning(tr_lines_table_.get(), "Error!", e.what());

    manager_->Remove(gui::ObjectType::TrappyLines, static_cast<size_t>(column));
    UpdateTable(gui::ObjectType::TrappyLines);
    area_->ReDraw();
  }
}

// MARK: Remove Items

void TablesConnection::RemoveTargetItem() {
  DisableTablesConnections_();

  manager_->Remove(gui::ObjectType::Targets, selected_column_);
  area_->ReDraw();
  UpdateTable(gui::ObjectType::Targets);
  UpdateTable(gui::ObjectType::TrappyLines);

  UpdateTablesConnections_();
}

void TablesConnection::RemoveHillItem() {
  DisableTablesConnections_();

  manager_->Remove(gui::ObjectType::Hills, selected_column_);
  area_->ReDraw();
  UpdateTable(gui::ObjectType::Hills);

  UpdateTablesConnections_();
}

void TablesConnection::RemoveTrappyCircleItem() {
  DisableTablesConnections_();

  manager_->Remove(gui::ObjectType::TrappyCircles, selected_column_);
  area_->ReDraw();
  UpdateTable(gui::ObjectType::TrappyCircles);

  UpdateTablesConnections_();
}

void TablesConnection::RemoveTrappyLineItem() {
  DisableTablesConnections_();

  manager_->Remove(gui::ObjectType::TrappyLines, selected_column_);
  area_->ReDraw();
  UpdateTable(gui::ObjectType::TrappyLines);

  UpdateTablesConnections_();
}

// MARK: Update Connections

void TablesConnection::UpdateTablesConnections_() {
  QObject::connect(targets_table_.get(), &QTableWidget::cellChanged, this,
                   &TablesConnection::TargetsItemChanged);

  QObject::connect(hills_table_.get(), &QTableWidget::cellChanged, this,
                   &TablesConnection::HillsItemChanged);

  QObject::connect(tr_circles_table_.get(), &QTableWidget::cellChanged, this,
                   &TablesConnection::TrappyCirclesItemChanged);

  QObject::connect(tr_lines_table_.get(), &QTableWidget::cellChanged, this,
                   &TablesConnection::TrappyLinesItemChanged);
}

void TablesConnection::DisableTablesConnections_() {
  QObject::disconnect(targets_table_.get(), &QTableWidget::cellChanged, this,
                      &TablesConnection::TargetsItemChanged);

  QObject::disconnect(hills_table_.get(), &QTableWidget::cellChanged, this,
                      &TablesConnection::HillsItemChanged);

  QObject::disconnect(tr_circles_table_.get(), &QTableWidget::cellChanged, this,
                      &TablesConnection::TrappyCirclesItemChanged);

  QObject::disconnect(tr_lines_table_.get(), &QTableWidget::cellChanged, this,
                      &TablesConnection::TrappyLinesItemChanged);
}

void TablesConnection::UpdateRemoveButtonConnections_() {
  // активируем кнопки при выборе любой клетки
  QObject::connect(targets_table_.get(), &QTableWidget::cellClicked, this,
                   &TablesConnection::EnableRemoveTargetButton);

  QObject::connect(hills_table_.get(), &QTableWidget::cellClicked, this,
                   &TablesConnection::EnableRemoveHillButton);

  QObject::connect(tr_lines_table_.get(), &QTableWidget::cellClicked, this,
                   &TablesConnection::EnableRemoveTrappyLineButton);

  QObject::connect(tr_circles_table_.get(), &QTableWidget::cellClicked, this,
                   &TablesConnection::EnableRemoveTrappyCircleButton);

  // деактивируем кнопки, если выбор изменился
  QObject::connect(targets_table_.get(), &QTableWidget::itemSelectionChanged,
                   this, &TablesConnection::DisableRemoveTargetButton);

  QObject::connect(hills_table_.get(), &QTableWidget::itemSelectionChanged,
                   this, &TablesConnection::DisableRemoveHillButton);

  QObject::connect(tr_lines_table_.get(), &QTableWidget::itemSelectionChanged,
                   this, &TablesConnection::DisableRemoveTrappyLineButton);

  QObject::connect(tr_circles_table_.get(), &QTableWidget::itemSelectionChanged,
                   this, &TablesConnection::DisableRemoveTrappyCircleButton);

  // привязываем сами функции к сигналам
  QObject::connect(targets_remove_button_.get(), &QPushButton::clicked, this,
                   &TablesConnection::RemoveTargetItem);

  QObject::connect(hills_remove_button_.get(), &QPushButton::clicked, this,
                   &TablesConnection::RemoveHillItem);

  QObject::connect(tr_circles_remove_button_.get(), &QPushButton::clicked, this,
                   &TablesConnection::RemoveTrappyCircleItem);

  QObject::connect(tr_lines_remove_button_.get(), &QPushButton::clicked, this,
                   &TablesConnection::RemoveTrappyLineItem);
}

}  // namespace data_tools
