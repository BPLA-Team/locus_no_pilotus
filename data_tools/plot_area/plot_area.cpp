// header file
#include "plot_area.h"

namespace data_tools {

void PlotArea::Setup(DataManager* manager) { manager_.reset(manager); }

void PlotArea::ReDraw() {
  plot_->clearPlottables();
  plot_->clearItems();

  for (auto& target : manager_->GetTargetsPtrs()) target->Draw(plot_.get());

  for (auto& hill : manager_->GetHillsPtrs()) hill->Draw(plot_.get());

  for (auto& tr_circle : manager_->GetTrappyCirclesPtrs())
    tr_circle->Draw(plot_.get());

  for (auto& tr_line : manager_->GetTrappyLinesPtrs())
    tr_line->Draw(plot_.get());

  trajectory_->Clear();

  plot_->replot();
}

void PlotArea::ReDrawTrajectory() {
  ReDraw();

  try {
    CheckTrappyCircles_();
    CheckHills_();
    CheckTrappyLines_();
    CheckTargets_();

    CalculateTrajectory_();
    trajectory_->Draw(plot_.get());
    plot_->replot();
  } catch (const std::exception& e) {
    QMessageBox::warning(plot_.get(), "Cannot calculate trajectory!", e.what());
  }
}

void PlotArea::CalculateTrajectory_() {
  auto lib_targets = std::vector<lib::Target>(manager_->GetTargets().size());
  for (size_t i = 0; i < lib_targets.size(); i++)
    lib_targets[i] = manager_->GetTargets()[i].GetData();

  auto lib_hills = std::vector<lib::Hill>(manager_->GetHills().size());
  for (size_t i = 0; i < lib_hills.size(); i++)
    lib_hills[i] = manager_->GetHills()[i].GetData();

  auto lib_tr_lines =
      std::vector<lib::TrappyLine>(manager_->GetTrappyLines().size());
  for (size_t i = 0; i < lib_tr_lines.size(); i++)
    lib_tr_lines[i] = manager_->GetTrappyLines()[i].GetData();

  auto lib_tr_circles =
      std::vector<lib::TrappyCircle>(manager_->GetTrappyCircles().size());
  for (size_t i = 0; i < lib_tr_circles.size(); i++)
    lib_tr_circles[i] = manager_->GetTrappyCircles()[i].GetData();

  trajectory_->Calculate(lib_targets, lib_hills, lib_tr_circles, lib_tr_lines,
                         amount_of_robots_);
  if (lib_targets.size() > 1) robot_->SetTrajectory(trajectory_.get());
}

}  // namespace data_tools
