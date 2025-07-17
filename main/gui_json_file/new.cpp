// header file:
#include "gui_json_file.h"

/**
 * @brief Нужно для того, чтобы при открытии приложения менялось имя "Untitled"
 * файла, так как может быть такое, что пользователь уже создавал такие файлы
 */
void GuiJsonFile::SetUntitledFile() {
  std::filesystem::path path;
  if (file_->filesystemFileName().has_parent_path())
    path = file_->filesystemFileName().parent_path();
  else
    path = std::filesystem::current_path();
  file_->setFileName(path / "Untitled.json");

  int num = 2;
  while (file_->exists()) {
    std::string untitled_filename =
        "Untitled (" + std::to_string(num) + ").json";
    file_->setFileName(path / untitled_filename);
    num++;
  }
}

QJsonObject GuiJsonFile::LoadJson_() const {
  QString json_text = file_->readAll();
  QJsonDocument json_file = QJsonDocument::fromJson(json_text.toUtf8());
  return json_file.object();
}

bool GuiJsonFile::IsChanged(data_tools::DataManager* manager) const {
  file_->open(QIODevice::ReadOnly | QFile::Text);
  QJsonObject root = LoadJson_();

  QJsonArray json_targets = root["Targets"].toArray();
  QJsonArray json_trappy_circles = root["Trappy_Circles"].toArray();
  QJsonArray json_trappy_lines = root["Trappy_Lines"].toArray();
  QJsonArray json_hills = root["Hills"].toArray();

  if (manager->GetTargets().size() ==
          static_cast<size_t>(json_targets.size()) &&
      manager->GetTrappyCircles().size() ==
          static_cast<size_t>(json_trappy_circles.size()) &&
      manager->GetTrappyLines().size() ==
          static_cast<size_t>(json_trappy_lines.size()) &&
      manager->GetHills().size() == static_cast<size_t>(json_hills.size())) {
    for (size_t i = 0; i < manager->GetTargets().size(); i++) {
      lib::Target t = manager->GetTargets()[i].GetData();
      if (t.IsChanged(json_targets.at(i).toObject())) {
        file_->close();
        return true;
      }
    }

    for (size_t i = 0; i < manager->GetTrappyCircles().size(); i++) {
      lib::TrappyCircle tc = manager->GetTrappyCircles()[i].GetData();
      if (tc.IsChanged(json_trappy_circles.at(i).toObject())) {
        file_->close();
        return true;
      }
    }

    for (size_t i = 0; i < manager->GetTrappyLines().size(); i++) {
      lib::TrappyLine tl = manager->GetTrappyLines()[i].GetData();
      if (tl.IsChanged(json_trappy_lines.at(i).toObject())) {
        file_->close();
        return true;
      }
    }

    for (size_t i = 0; i < manager->GetHills().size(); i++) {
      lib::Hill h = manager->GetHills()[i].GetData();
      if (h.IsChanged(json_hills.at(i).toObject())) {
        file_->close();
        return true;
      }
    }

    file_->close();
    return false;
  }

  file_->close();
  return true;
}
