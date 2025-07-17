// header file:
#include "trappy_line.h"

namespace gui {

void TrappyLine::Draw(QCustomPlot* plot) {
  graph_ = plot->addGraph(plot->xAxis, plot->yAxis);

  QPen pen;
  pen.setColor(QColor(200, 50, 50, 255));
  pen.setStyle(Qt::DashLine);
  graph_->setPen(pen);

  graph_->setLineStyle(QCPGraph::lsLine);
  graph_->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 8));

  auto targets = data_.GetTargets();

  graph_->addData(targets.first.GetPoint().x, targets.first.GetPoint().y);
  graph_->addData(targets.second.GetPoint().x, targets.second.GetPoint().y);
}

void TrappyLine::UpdateData_(gui::Target* first_target,
                             gui::Target* second_target) {
  data_.SetTargets(&first_target->GetData(), &second_target->GetData());

  targets_.first = first_target;
  targets_.second = second_target;
}

void TrappyLine::UpdateData_(std::pair<gui::Target*, gui::Target*> targets) {
  UpdateData_(targets.first, targets.second);
}

}  // namespace gui
