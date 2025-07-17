// header file:
#include "main/mainwindow.h"

void MainWindow::DisconnectObject(gui::ObjectType obj_type) {
  switch (obj_type) {
    case gui::ObjectType::TrappyCircles: {
      disconnect(ui->plot, &QCustomPlot::mouseMove, this,
                 &MainWindow::mouseMoveSetRadiusFromPlot);
      disconnect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
                 &MainWindow::mousePressSetRadiusFromPlot);
      break;
    }

    case gui::ObjectType::TrappyLines: {
      disconnect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
                 &MainWindow::mousePressSelectSecondTarget);
      break;
    }

    case gui::ObjectType::Hills: {
      disconnect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
                 &MainWindow::mousePressAddVertice);
      disconnect(ui->plot, &QCustomPlot::mousePress, this,
                 &MainWindow::mousePressDeleteLastVertice);
      disconnect(ui->plot, &QCustomPlot::mouseMove, this,
                 &MainWindow::mouseMoveAddVertice);
      break;
    }

    default:
      break;
  }

  disconnect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
             &MainWindow::mousePressObjectsButton);

  connect(ui->plot, &QCustomPlot::mousePress, this,
          &MainWindow::mousePressContextMenu);

  ui->plot->setCursor(Qt::CrossCursor);
  cursor_ = CursorType::DefaultCursor;
}

/**
 * @brief Удаляет последний добавленный объект с помощью кнопок слева,
 * причем его добавление не закончилось. Она должна вызывается везде, где
 * происходит взаимодействие пользователя с кнопками класса "MainWindow"
 */
void MainWindow::DeleteLastAddedObject_() {
  DisconnectObject(GetObjType_());
  switch (what_obj_addition_) {
    case WhatObjectAddition::TrCircle: {
      size_t last = manager_->GetTrappyCircles().size() - 1;
      manager_->Remove(gui::ObjectType::TrappyCircles, last);
      break;
    }

    case WhatObjectAddition::TrLine: {
      size_t last = manager_->GetTrappyLines().size() - 1;
      manager_->Remove(gui::ObjectType::TrappyLines, last);
      break;
    }

    case WhatObjectAddition::Hill: {
      size_t last = manager_->GetHills().size() - 1;
      manager_->Remove(gui::ObjectType::Hills, last);
      break;
    }

    default:
      break;
  }

  area_->ReDraw();
  DeCalcTrajectory_();

  what_obj_addition_ = WhatObjectAddition::Nothing;
}

void MainWindow::keyPressEvent(QKeyEvent* key_event) {
  if (key_event->key() == Qt::Key_Escape) {
    // Проверка на то, что пользователь уже начал создавать объект,
    // поэтому при нажатии Esc мы должны удалять последний добавленный объект
    DeleteLastAddedObject_();
  }
}

void MainWindow::mousePressObjectsButton(QMouseEvent* mouse_event) {
  if (mouse_event->button() != Qt::LeftButton) return;
  disconnect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
             &MainWindow::mousePressObjectsButton);

  double x = ui->plot->xAxis->pixelToCoord(mouse_event->pos().x());
  double y = ui->plot->yAxis->pixelToCoord(mouse_event->pos().y());

  try {
    switch (cursor_) {
      case CursorType::TargetCursor: {
        manager_->Add(new gui::Target(x, y));

        ui->plot->setCursor(Qt::CrossCursor);
        cursor_ = CursorType::DefaultCursor;

        // после финального добавления обновляем таблицу
        t_connection_->UpdateTable(gui::ObjectType::Targets);

        break;
      }

      case CursorType::TrCircleCursor: {
        manager_->Add(new gui::TrappyCircle({x, y}, 0));
        disconnect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
                   &MainWindow::mousePressObjectsButton);
        disconnect(ui->plot, &QCustomPlot::mousePress, this,
                   &MainWindow::mousePressContextMenu);

        connect(ui->plot, &QCustomPlot::mouseMove, this,
                &MainWindow::mouseMoveSetRadiusFromPlot);
        connect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
                &MainWindow::mousePressSetRadiusFromPlot);

        ui->plot->setCursor(Qt::ClosedHandCursor);
        what_obj_addition_ = WhatObjectAddition::TrCircle;
        break;
      }

      case CursorType::TrLineCursor: {
        if (ui->plot->selectedGraphs().empty()) return;

        for (const auto& t_ptr : manager_->GetTargetsPtrs()) {
          if (t_ptr->GetGraphPtr() == ui->plot->selectedGraphs()[0]) {
            manager_->Add(new gui::TrappyLine(t_ptr, t_ptr));
            disconnect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
                       &MainWindow::mousePressObjectsButton);
            disconnect(ui->plot, &QCustomPlot::mousePress, this,
                       &MainWindow::mousePressContextMenu);

            connect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
                    &MainWindow::mousePressSelectSecondTarget);

            what_obj_addition_ = WhatObjectAddition::TrLine;
            break;
          }
        }
        break;
      }

      case CursorType::HillCursor: {
        manager_->Add(new gui::Hill{{x, y}, {x, y}});

        disconnect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
                   &MainWindow::mousePressObjectsButton);
        disconnect(ui->plot, &QCustomPlot::mousePress, this,
                   &MainWindow::mousePressContextMenu);

        connect(ui->plot, &QCustomPlot::mouseDoubleClick, this,
                &MainWindow::mousePressAddVertice);
        connect(ui->plot, &QCustomPlot::mousePress, this,
                &MainWindow::mousePressDeleteLastVertice);
        connect(ui->plot, &QCustomPlot::mouseMove, this,
                &MainWindow::mouseMoveAddVertice);

        what_obj_addition_ = WhatObjectAddition::Hill;
        break;
      }

      default:
        break;
    }

    area_->ReDraw();

  } catch (const std::exception& e) {
    QMessageBox::critical(this, "Error!", e.what());
  }
}

void MainWindow::mousePressSetRadiusFromPlot(QMouseEvent* mouse_event) {
  if (mouse_event->button() == Qt::LeftButton) {
    DisconnectObject(gui::ObjectType::TrappyCircles);
    what_obj_addition_ = WhatObjectAddition::Nothing;

    area_->ReDraw();

    // после финального добавления обновляем таблицу
    t_connection_->UpdateTable(gui::ObjectType::TrappyCircles);
  }
}

void MainWindow::mouseMoveSetRadiusFromPlot(QMouseEvent* mouse_event) {
  size_t last = manager_->GetTrappyCircles().size() - 1;

  // Расстояние от центра до курсора (по оси x)
  double x = abs(ui->plot->xAxis->pixelToCoord(mouse_event->pos().x()) -
                 manager_->GetTrappyCircles()[last].GetCenter().x);

  // Расстояние от центра до курсора (по оси y)
  double y = abs(ui->plot->yAxis->pixelToCoord(mouse_event->pos().y()) -
                 manager_->GetTrappyCircles()[last].GetCenter().y);

  // Значение радиуса
  double r = sqrt(pow(x, 2) + pow(y, 2));

  manager_->GetTrappyCirclesPtrs()[last]->SetRadius(r);
  area_->ReDraw();
}

void MainWindow::mousePressSelectSecondTarget(QMouseEvent* mouse_event) {
  if (mouse_event->button() != Qt::LeftButton) return;
  if (ui->plot->selectedGraphs().empty()) return;

  for (const auto& t_ptr : manager_->GetTargetsPtrs()) {
    if (t_ptr->GetGraphPtr() != ui->plot->selectedGraphs()[0]) continue;

    size_t last = manager_->GetTrappyLines().size() - 1;

    // если это та же точка - пропускаем
    if (manager_->GetTrappyLinesPtrs()[last]->GetTargetsPtrs().second == t_ptr)
      continue;

    manager_->GetTrappyLinesPtrs()[last]->SetSecondTarget(t_ptr);

    DisconnectObject(gui::ObjectType::TrappyLines);
    what_obj_addition_ = WhatObjectAddition::Nothing;

    area_->ReDraw();

    // после финального добавления обновляем таблицу
    t_connection_->UpdateTable(gui::ObjectType::TrappyLines);

    return;
  }
}

void MainWindow::mousePressAddVertice(QMouseEvent* mouse_event) {
  if (mouse_event->button() == Qt::LeftButton) {
    // Позиция курсора в пикселях
    int x_pixels = mouse_event->pos().x();
    int y_pixels = mouse_event->pos().y();

    // Позиция курсора в координатах
    double x = ui->plot->xAxis->pixelToCoord(x_pixels);
    double y = ui->plot->yAxis->pixelToCoord(y_pixels);

    size_t last = manager_->GetHills().size() - 1;

    // Позиция начальной точки в пикселях
    double x2_pixels = ui->plot->xAxis->coordToPixel(
        manager_->GetHills()[last].GetVertices()[0].x);
    double y2_pixels = ui->plot->yAxis->coordToPixel(
        manager_->GetHills()[last].GetVertices()[0].y);

    // Проверка на то, что расстояние от курсора до начальной точки меньше 10
    // пикселей. Если это так, то мы считаем, что он завершил создание Hill
    if (sqrt(pow(x_pixels - x2_pixels, 2) + pow(y_pixels - y2_pixels, 2)) <
            10 &&
        manager_->GetHills()[last].GetVertices().size() > 3) {
      size_t last_vertice =
          manager_->GetHillsPtrs()[last]->GetVertices().size() - 1;
      manager_->GetHillsPtrs()[last]->GetVertices().erase(
          manager_->GetHillsPtrs()[last]->GetVertices().begin() + last_vertice);
      DisconnectObject(gui::ObjectType::Hills);
      what_obj_addition_ = WhatObjectAddition::Nothing;

      // после финального добавления обновляем таблицу
      t_connection_->UpdateTable(gui::ObjectType::Hills);

    } else if (manager_->GetHills()[last].GetVertices()[0] ==
               manager_->GetHills()[last].GetVertices()[1]) {
      size_t last_p = manager_->GetHills()[last].GetVertices().size() - 1;
      manager_->GetHillsPtrs()[last]->GetVertices()[last_p].x = x;
      manager_->GetHillsPtrs()[last]->GetVertices()[last_p].y = y;

    } else
      manager_->GetHillsPtrs()[last]->AddVertice({x, y});

    area_->ReDraw();
  }
}

void MainWindow::mousePressDeleteLastVertice(QMouseEvent* mouse_event) {
  if (mouse_event->button() == Qt::RightButton) {
    size_t last = manager_->GetHills().size() - 1;
    size_t vertices_size = manager_->GetHills()[last].GetVertices().size();
    if (vertices_size > 2)
      manager_->GetHillsPtrs()[last]->GetVertices().pop_back();

    else {
      if (manager_->GetHills()[last].GetVertices()[0] !=
          manager_->GetHills()[last].GetVertices()[1]) {
        manager_->GetHillsPtrs()[last]->GetVertices()[1] =
            manager_->GetHills()[last].GetVertices()[0];

      } else
        DeleteLastAddedObject_();
    }

    area_->ReDraw();
  }
}

void MainWindow::mouseMoveAddVertice(QMouseEvent* mouse_event) {
  size_t last = manager_->GetHills().size() - 1;
  size_t last_vertice =
      manager_->GetHillsPtrs()[last]->GetVertices().size() - 1;

  if (last_vertice > 0) {
    double x = ui->plot->xAxis->pixelToCoord(mouse_event->pos().x());
    double y = ui->plot->yAxis->pixelToCoord(mouse_event->pos().y());

    manager_->GetHillsPtrs()[last]->GetVertices().erase(
        manager_->GetHillsPtrs()[last]->GetVertices().begin() + last_vertice);

    manager_->GetHillsPtrs()[last]->AddVertice({x, y});
    area_->ReDraw();
  }
}

void MainWindow::on_homeScalePushButton_clicked() {
  lib::Point first_target = {0, 0};
  double max_distance = 10;

  if (!manager_->GetTargets().empty()) {
    first_target = manager_->GetTargets()[0].GetPoint();
    if (manager_->GetTargets().size() > 1)
      for (size_t i = 1; i < manager_->GetTargets().size(); i++) {
        double distance = lib::DistanceBetweenPoints(
            first_target, manager_->GetTargets()[i].GetPoint());
        if (max_distance < distance) max_distance = distance;
      }
  }

  if (first_target.x - max_distance <= -max_scale ||
      first_target.x + max_distance >= max_scale ||
      first_target.y - max_distance <= -max_scale ||
      first_target.y + max_distance >= max_scale)
    max_distance /= 2.;
  ui->plot->xAxis->setRange(first_target.x - max_distance,
                            first_target.x + max_distance);
  ui->plot->yAxis->setRange(first_target.y - max_distance,
                            first_target.y + max_distance);
  on_actionBeautify_triggered();
}
