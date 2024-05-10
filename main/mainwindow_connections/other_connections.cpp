// header file:
#include "main/mainwindow.h"

gui::ObjectType MainWindow::GetObjType() const {
  switch (cursor_) {
    case CursorType::TrCircleCursor:
      return gui::ObjectType::TrappyCircles;
    case CursorType::TrLineCursor:
      return gui::ObjectType::TrappyLines;
    case CursorType::HillCursor:
      return gui::ObjectType::Hills;
    default:
      return gui::ObjectType::Targets;
  }
}

void MainWindow::on_addTargetPushButton_clicked() {
  DeleteLastAddedObject();
  connect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
          &MainWindow::mousePressObjectsButton);

  ui->plot->setCursor(QCursor(QPixmap("../images/better_cross_cursor.png")
                                  .scaled(QSize(24, 24), Qt::KeepAspectRatio)));
  cursor_ = CursorType::TargetCursor;
}

void MainWindow::on_addTrappyCirclePushButton_clicked() {
  DeleteLastAddedObject();
  connect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
          &MainWindow::mousePressObjectsButton);

  ui->plot->setCursor(QCursor(QPixmap("../images/move_cursor.png")
                                  .scaled(QSize(24, 24), Qt::KeepAspectRatio)));
  cursor_ = CursorType::TrCircleCursor;
}

void MainWindow::on_addTrappyLinePushButton_clicked() {
  DeleteLastAddedObject();
  connect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
          &MainWindow::mousePressObjectsButton);

  ui->plot->setCursor(QCursor(QPixmap("../images/choose_cursor.png")
                                  .scaled(QSize(24, 24), Qt::KeepAspectRatio)));
  cursor_ = CursorType::TrLineCursor;
}

void MainWindow::on_addHillPushButton_clicked() {
  DeleteLastAddedObject();
  connect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
          &MainWindow::mousePressObjectsButton);

  ui->plot->setCursor(QCursor(QPixmap("../images/move_cursor.png")
                                  .scaled(QSize(24, 24), Qt::KeepAspectRatio)));
  cursor_ = CursorType::HillCursor;
}

void MainWindow::on_editObjectsPushButton_clicked() {
  manager_->RemoveAllDuplicates();
  t_connection_->UpdateTables();

  ui->plotSettingsDockWidget->setVisible(true);
  on_actionBeautify_triggered();
}

void MainWindow::on_actionBeautify_triggered() {
  ui->plot->xAxis->setScaleRatio(ui->plot->yAxis);
  area_->Redraw();
}

void MainWindow::on_targetAddFromTablePushButton_clicked() {
  on_actionTarget_triggered();
}

void MainWindow::on_hillAddFromTablePushButton_clicked() {
  on_actionHill_triggered();
}

void MainWindow::on_trappyCircleAddFromTablePushButton_clicked() {
  on_actionTrappy_Circle_triggered();
}

void MainWindow::on_trappyLineAddFromTablePushButton_clicked() {
  on_actionTrappy_Line_triggered();
}

void MainWindow::on_xAxis_rangeChanged(QCPRange range) {
  if (range.upper > max_scale) {
    range.upper = max_scale;
    range.lower = max_scale - (range.upper - range.lower);
  }

  if (range.lower < -max_scale) {
    range.lower = -max_scale;
    range.upper = -max_scale + (range.upper - range.lower);
  }

  if (range.upper - range.lower < min_scale) {
    double mid = (range.upper + range.lower) / 2;
    range.lower = mid - min_scale / 2;
    range.upper = mid + min_scale / 2;
  }

  ui->plot->xAxis->setRange(range);
}

void MainWindow::on_yAxis_rangeChanged(QCPRange range) {
  if (range.upper > max_scale) {
    range.upper = max_scale;
    range.lower = max_scale - (range.upper - range.lower);
  }

  if (range.lower < -max_scale) {
    range.lower = -max_scale;
    range.upper = -max_scale + (range.upper - range.lower);
  }

  if (range.upper - range.lower < min_scale) {
    double mid = (range.upper + range.lower) / 2;
    range.lower = mid - min_scale / 2;
    range.upper = mid + min_scale / 2;
  }

  ui->plot->yAxis->setRange(range);
}

void MainWindow::mousePressRemoveObject() {
  if (ui->plot->selectedGraphs().size() > 0) {
    bool is_found = false;
    for (size_t i = 0; i < manager_->GetTargets().size(); i++) {
      if (ui->plot->selectedGraphs()[0] ==
          manager_->GetTargetsPtrs()[i]->GetGraphPtr()) {
        manager_->Remove(gui::ObjectType::Targets, i);
        t_connection_->UpdateTable(gui::ObjectType::Targets);
        is_found = true;
        break;
      }
    }

    if (!is_found)
      for (size_t i = 0; i < manager_->GetTrappyLines().size(); i++) {
        if (ui->plot->selectedGraphs()[0] ==
            manager_->GetTrappyLinesPtrs()[i]->GetGraphPtr()) {
          manager_->Remove(gui::ObjectType::TrappyLines, i);
          t_connection_->UpdateTable(gui::ObjectType::TrappyLines);
          break;
        }
      }

  } else if (ui->plot->selectedPlottables().size() > 0) {
    for (size_t i = 0; i < manager_->GetHills().size(); i++) {
      if (ui->plot->selectedPlottables()[0] ==
          manager_->GetHillsPtrs()[i]->GetCurvePtr()) {
        manager_->Remove(gui::ObjectType::Hills, i);
        t_connection_->UpdateTable(gui::ObjectType::Hills);
        break;
      }
    }

  } else if (ui->plot->selectedItems().size() > 0)
    for (size_t i = 0; i < manager_->GetTrappyCircles().size(); i++) {
      if (ui->plot->selectedItems()[0] ==
          manager_->GetTrappyCirclesPtrs()[i]->GetItemEllipsePtr()) {
        manager_->Remove(gui::ObjectType::TrappyCircles, i);
        t_connection_->UpdateTable(gui::ObjectType::TrappyCircles);
        break;
      }
    }

  area_->Redraw();
}

void MainWindow::mousePressContextMenu(QMouseEvent* mouse_event) {
  if (mouse_event->button() == Qt::RightButton &&
      (!ui->plot->selectedGraphs().empty() ||
       !ui->plot->selectedItems().empty() ||
       !ui->plot->selectedPlottables().empty())) {
    ui->plot->setContextMenuPolicy(Qt::ActionsContextMenu);

    QMenu* menu{new QMenu(this)};
    QAction* remove_button{new QAction("Remove", this)};
    menu->addAction(remove_button);
    connect(remove_button, &QAction::triggered, this,
            &MainWindow::mousePressRemoveObject);

    menu->exec(mouse_event->globalPosition().toPoint());
  }
}
