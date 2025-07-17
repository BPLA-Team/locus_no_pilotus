// header file:
#include "main/mainwindow.h"

/**
 * @brief Вызов окна, которое сообщает об изменениях в файле
 * @return true: если окно было закрыто
 */
bool MainWindow::OpenMessageWindow_() {
  QString text =
      "Do you want save changes in file " + json_file_.GetFileName() + "?";
  int ret = QMessageBox::question(
      this, "Are you sure?", text,
      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

  switch (ret) {
    case QMessageBox::Save: {
      bool is_closed = on_saveFileAction_triggered();
      if (is_closed) return true;
      break;
    }

    case QMessageBox::Discard:
      break;

    case QMessageBox::Cancel:
    case QMessageBox::Close:
      return true;
      break;
  }

  return false;
}

/// @brief Проверяет при закрытии приложения, есть ли изменения в текущем файле
void MainWindow::closeEvent(QCloseEvent* event) {
  DeleteLastAddedObject_();

  bool is_closed = false;

  if (json_file_.IsExistsFile() && json_file_.IsChanged(manager_.get()))
    is_closed = OpenMessageWindow_();

  else if (!json_file_.IsExistsFile() && (manager_->GetTargets().size() +
                                          manager_->GetTrappyCircles().size() +
                                          manager_->GetTrappyLines().size() +
                                          manager_->GetHills().size()) != 0) {
    is_closed = OpenMessageWindow_();
  }

  if (is_closed)
    event->ignore();
  else
    event->accept();
}

/// @brief Кнопка "New"
void MainWindow::on_newFileAction_triggered() {
  DeleteLastAddedObject_();

  bool is_closed = false;

  if (json_file_.IsExistsFile() && json_file_.IsChanged(manager_.get()))
    is_closed = OpenMessageWindow_();

  else if (!json_file_.IsExistsFile() && (manager_->GetTargets().size() +
                                          manager_->GetTrappyCircles().size() +
                                          manager_->GetTrappyLines().size() +
                                          manager_->GetHills().size()) != 0)
    is_closed = OpenMessageWindow_();

  if (!is_closed) {
    manager_->Clear();
    json_file_.SetUntitledFile();
    area_->ReDraw();
    t_connection_->UpdateTables();
  }
}

/// @brief Кнопка "Open"
void MainWindow::on_openFileAction_triggered() {
  DeleteLastAddedObject_();

  bool is_closed = false;

  if (json_file_.IsExistsFile() && json_file_.IsChanged(manager_.get()))
    is_closed = OpenMessageWindow_();

  else if (!json_file_.IsExistsFile() && (manager_->GetTargets().size() +
                                          manager_->GetTrappyCircles().size() +
                                          manager_->GetTrappyLines().size() +
                                          manager_->GetHills().size()) != 0)
    is_closed = OpenMessageWindow_();

  if (is_closed) return;

  QString file_name = QFileDialog::getOpenFileName(
      this, tr("Open"), json_file_.GetParentPath(), tr("File (*.json)"));

  if (file_name.isEmpty()) return;

  QString old_filename = json_file_.GetAbsolutePath();

  try {
    json_file_.SetFile(file_name);
    manager_->Clear();
    json_file_.Open(manager_.get());

  } catch (const std::exception& e) {
    QMessageBox::critical(this, "Error!", e.what());
    json_file_.Close();
    manager_->Clear();
    json_file_.SetFile(old_filename);
    json_file_.Open(manager_.get());
  }

  area_->ReDraw();
  t_connection_->UpdateTables();
}

/**
 * @brief Кнопка "Save"
 * @return true: закрыто окно сохранения файла
 */
bool MainWindow::on_saveFileAction_triggered() {
  DeleteLastAddedObject_();

  if (!json_file_.IsExistsFile())
    return on_saveAsFileAction_triggered();
  else {
    json_file_.Save(manager_.get());
    return false;
  }
}

/**
 * @brief Кнопка "Save as"
 * @return true: закрыто окно сохранения файла
 */
bool MainWindow::on_saveAsFileAction_triggered() {
  DeleteLastAddedObject_();

  QString file_name = QFileDialog::getSaveFileName(
      this, tr("Save as"), json_file_.GetAbsolutePath(), tr("File (*.json)"));

  if (!file_name.isEmpty()) {
    json_file_.SetFile(file_name);
    json_file_.Save(manager_.get());
    return false;
  }
  return true;
}
