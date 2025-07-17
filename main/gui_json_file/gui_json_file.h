#pragma once

// std libs:
#include <filesystem>

// our code libs:
#include "data_tools/data_manager/data_manager.h"

class GuiJsonFile {
 public:
  GuiJsonFile() = default;
  GuiJsonFile(QFile* file) : file_{file} {};

  void Save(data_tools::DataManager* plot_area);
  void Open(data_tools::DataManager* plot_area);
  void Close() { file_->close(); }

  QString GetFileName() const {
    return QString::fromStdString(
        file_->filesystemFileName().filename().string());
  }

  QString GetAbsolutePath() const {
    return QString::fromStdString(file_->filesystemFileName().string());
  }

  QString GetParentPath() const {
    return QString::fromStdString(
        file_->filesystemFileName().parent_path().string());
  }

  void SetFile(const QString& file_name) { file_->setFileName(file_name); }
  void SetUntitledFile();

  bool IsExistsFile() const { return file_->exists(); }
  bool IsChanged(data_tools::DataManager* plot_area) const;

 private:
  QFile* file_{new QFile};

  QJsonObject LoadJson() const;
};
