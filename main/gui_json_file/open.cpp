// header file:
#include "gui_json_file.h"

std::vector<lib::Target> GetTargetsFromFile(QJsonArray arr) {
  std::vector<lib::Target> targets;
  for (size_t i = 0; i < static_cast<size_t>(arr.size()); i++) {
    lib::Target t;
    t.SetJsonInfo(arr.at(i).toObject());
    targets.push_back(t);
  }
  return targets;
}

std::vector<lib::TrappyCircle> GetTrappyCirclesFromFile(QJsonArray arr) {
  std::vector<lib::TrappyCircle> trappy_circles;
  for (size_t i = 0; i < static_cast<size_t>(arr.size()); i++) {
    lib::TrappyCircle tc;
    tc.SetJsonInfo(arr.at(i).toObject());
    trappy_circles.push_back(tc);
  }
  return trappy_circles;
}

std::vector<lib::Hill> GetHillsFromFile(QJsonArray arr) {
  std::vector<lib::Hill> hills;
  for (size_t i = 0; i < static_cast<size_t>(arr.size()); i++) {
    lib::Hill h;
    h.SetJsonInfo(arr.at(i).toObject());
    hills.push_back(h);
  }
  return hills;
}

bool IsExistId(const std::vector<unsigned short>& ids, unsigned short curr_id) {
  for (const auto& id : ids) {
    if (curr_id == id) return true;
  }
  return false;
}

void GuiJsonFile::Open(data_tools::DataManager* manager) {
  if (file_->open(QIODevice::ReadOnly | QFile::Text)) {
    QJsonObject root = LoadJson_();

    if (!(root.contains("Targets") && root.contains("Trappy_Circles") &&
          root.contains("Trappy_Lines") && root.contains("Hills")))
      throw std::invalid_argument(
          "Invalid file format: missing some objects fields!");

    QJsonArray json_targets = root["Targets"].toArray();
    QJsonArray json_trappy_circles = root["Trappy_Circles"].toArray();
    QJsonArray json_trappy_lines = root["Trappy_Lines"].toArray();
    QJsonArray json_hills = root["Hills"].toArray();

    std::vector<lib::Target> targets;

    std::vector<unsigned short> targets_ids;
    for (size_t i = 0; i < static_cast<size_t>(json_targets.size()); i++) {
      lib::Target t;
      t.SetJsonInfo(json_targets.at(i).toObject());
      targets.push_back(t);

      if (IsExistId(targets_ids, t.GetId()))
        throw std::invalid_argument(
            "Invalid file format: there are identical id's in 'Targets'!");

      targets_ids.push_back(t.GetId());
    }
    manager->Set(GetTargetsFromFile(json_targets));

    std::vector<unsigned short> trappy_circles_ids;
    for (size_t i = 0; i < static_cast<size_t>(json_trappy_circles.size());
         i++) {
      lib::TrappyCircle trc;
      trc.SetJsonInfo(json_trappy_circles.at(i).toObject());

      if (IsExistId(trappy_circles_ids, trc.GetId()))
        throw std::invalid_argument(
            "Invalid file format: there are identical id's in "
            "'Trappy_Circles'!");

      trappy_circles_ids.push_back(trc.GetId());
    }
    manager->Set(GetTrappyCirclesFromFile(json_trappy_circles));

    std::vector<unsigned short> trappy_lines_ids;
    for (size_t i = 0; i < static_cast<size_t>(json_trappy_lines.size()); i++) {
      QJsonObject json_tr_line = json_trappy_lines.at(i).toObject();

      unsigned short id1 =
          static_cast<unsigned short>(json_tr_line.value("Id_P1").toInt());
      unsigned short id2 =
          static_cast<unsigned short>(json_tr_line.value("Id_P2").toInt());

      std::pair<gui::Target*, gui::Target*> targets_ptrs;
      for (const auto& target : manager->GetTargetsPtrs()) {
        if (target->GetData().GetId() == id1) targets_ptrs.first = target;
        if (target->GetData().GetId() == id2) targets_ptrs.second = target;
      }

      if (targets_ptrs.first == nullptr || targets_ptrs.second == nullptr)
        throw std::invalid_argument(
            "Invalid file format: non-existent id for 'Target' in "
            "'Trappy_Line'!");

      gui::TrappyLine* trl{
          new gui::TrappyLine(targets_ptrs.first, targets_ptrs.second)};

      unsigned short id =
          static_cast<unsigned short>(json_tr_line.value("Id").toInt());
      if (IsExistId(trappy_lines_ids, id))
        throw std::invalid_argument(
            "Invalid file format: there are identical id's in "
            "'Trappy_Lines'!");
      if (id < 30000 || id > 39999)
        throw std::invalid_argument(
            "Invalid file format: incorrect id in 'Trappy_Line'!");

      trappy_lines_ids.push_back(id);
      trl->GetData().SetId(id);
      manager->Add(trl);
    }

    std::vector<unsigned short> hills_ids;
    for (size_t i = 0; i < static_cast<size_t>(json_hills.size()); i++) {
      lib::Hill h;
      h.SetJsonInfo(json_hills.at(i).toObject());

      if (IsExistId(hills_ids, h.GetId()))
        throw std::invalid_argument(
            "Invalid file format: there are identical id's in 'Hills'!");

      hills_ids.push_back(h.GetId());
    }
    manager->Set(GetHillsFromFile(json_hills));

    file_->close();
    // перерисовка произойдёт уже после
  }
}
