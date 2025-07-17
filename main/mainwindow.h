#pragma once

// Qt libs:
#include <QMainWindow>

// autogen libs:
#include "./ui_mainwindow.h"

// our code libs:
#include "add_objects_forms/_forms.h"
#include "data_tools/_tools.h"
#include "gui_json_file/gui_json_file.h"

enum class CursorType {
  TargetCursor,
  TrCircleCursor,
  TrLineCursor,
  HillCursor,
  DefaultCursor
};

constexpr double max_scale = lib::max_coord / 2;
constexpr double min_scale = 10e-4;

enum class WhatObjectAddition { Nothing, Target, TrCircle, TrLine, Hill };

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  // Slots for files:
  bool on_saveAsFileAction_triggered();
  bool on_saveFileAction_triggered();
  void on_openFileAction_triggered();
  void on_newFileAction_triggered();

  // Slots for interaction objects adding:
  void mousePressObjectsButton(QMouseEvent* mouse_event);
  void mousePressContextMenu(QMouseEvent* mouse_event);
  void mousePressRemoveObject();
  void DisconnectObject(gui::ObjectType obj_type);
  void closeEvent(QCloseEvent* event) override;
  void keyPressEvent(QKeyEvent* key_event) override;

  // Slots for Target:
  void on_addTargetPushButton_clicked();
  void on_targetAction_triggered();

  // Slots for TrappyCircle:
  void on_addTrappyCirclePushButton_clicked();
  void on_trappyCircleAction_triggered();
  void mousePressSetRadiusFromPlot(QMouseEvent* mouse_event);
  void mouseMoveSetRadiusFromPlot(QMouseEvent* mouse_event);

  // Slots for TrappyLine:
  void on_addTrappyLinePushButton_clicked();
  void on_trappyLineAction_triggered();
  void mousePressSelectSecondTarget(QMouseEvent* mouse_event);

  // Slots for Hill:
  void on_addHillPushButton_clicked();
  void on_hillAction_triggered();
  void mousePressAddVertice(QMouseEvent* mouse_event);
  void mousePressDeleteLastVertice(QMouseEvent* mouse_event);
  void mouseMoveAddVertice(QMouseEvent* mouse_event);

  // Slots for Settings:
  void on_targetAddFromTablePushButton_clicked();
  void on_hillAddFromTablePushButton_clicked();
  void on_trappyCircleAddFromTablePushButton_clicked();
  void on_trappyLineAddFromTablePushButton_clicked();
  void on_editObjectsPushButton_clicked();

  // Extra slots:
  void on_actionBeautify_triggered();

  void on_xAxis_rangeChanged(QCPRange range);
  void on_yAxis_rangeChanged(QCPRange range);

  void moveRobot();
  void on_calcTrajectoryPushButton_clicked();
  void on_flyRobotPushButton_clicked();
  void on_homeScalePushButton_clicked();
  void on_actionHelp_triggered();

  void on_robotsApplyAmountPushButton_clicked();

  void on_LowSpeedButton_clicked();
  void on_MediumSpeedButton_clicked();
  void on_HighSpeedButton_clicked();

 public slots:
  void AddTrappyCircle(double x, double y, double radius);
  void AddTarget(double x, double y);
  void AddTrappyLine(double x1, double y1, double x2, double y2);
  void AddHill(const std::vector<std::pair<double, double>>& points);

 private:
  Ui::MainWindow* ui;
  QTimer* timer_{new QTimer(this)};

  std::unique_ptr<data_tools::PlotArea> area_;
  std::unique_ptr<data_tools::DataManager> manager_;
  std::unique_ptr<data_tools::TablesConnection> t_connection_;

  GuiJsonFile json_file_;
  CursorType cursor_{CursorType::DefaultCursor};
  WhatObjectAddition what_obj_addition_{WhatObjectAddition::Nothing};

  bool is_robot_flying_ = false;
  bool is_drown_trajectory_ = false;

  bool OpenMessageWindow_();
  gui::ObjectType GetObjType_() const;
  void DeleteLastAddedObject_();

  void CalcTrajectory_();
  void DeCalcTrajectory_();

  void StopRobot_();
  void FlyRobot_();
};
