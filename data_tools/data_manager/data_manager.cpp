#include "data_manager.h"

namespace data_tools {

void DataManager::Remove(gui::ObjectType obj_type, size_t index) {
  switch (obj_type) {
    case gui::ObjectType::Targets: {
      targets_.erase(targets_.begin() + index);
      break;
    }

    case gui::ObjectType::Hills: {
      hills_.erase(hills_.begin() + index);
      break;
    }

    case gui::ObjectType::TrappyCircles: {
      tr_circles_.erase(tr_circles_.begin() + index);
      break;
    }

    case gui::ObjectType::TrappyLines: {
      tr_lines_.erase(tr_lines_.begin() + index);
      break;
    }
  }

  CheckErrorValues();
}

void DataManager::Clear() {
  targets_.clear();
  tr_circles_.clear();
  tr_lines_.clear();
  hills_.clear();

  CheckErrorValues();
}

// ----------------------   Target methods   ----------------------

void DataManager::Add(gui::Target* t) {
  targets_.emplace_back(t);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::initializer_list<gui::Target*> new_targets) {
  for (const auto& target : new_targets) Add(target);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(lib::Target data) {
  targets_.emplace_back(new gui::Target(data));

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::initializer_list<lib::Target> new_targets) {
  for (const auto& target : new_targets) Add(target);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::vector<gui::Target*> new_targets) {
  for (const auto& target : new_targets) Add(target);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::vector<lib::Target> new_targets) {
  for (const auto& target : new_targets) Add(target);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Set(std::initializer_list<gui::Target*> targets) {
  targets_.clear();
  Add(targets);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::initializer_list<lib::Target> targets) {
  targets_.clear();
  Add(targets);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::vector<gui::Target*> targets) {
  targets_.clear();
  Add(targets);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::vector<lib::Target> targets) {
  targets_.clear();
  Add(targets);

  CheckErrorValues();
  RemoveAllDuplicates();
}

std::vector<gui::Target*> DataManager::GetTargetsPtrs() {
  auto res = std::vector<gui::Target*>();
  for (auto& target_ptr_ : targets_) {
    res.push_back(target_ptr_.get());
  }

  return res;
}

std::vector<gui::Target> DataManager::GetTargets() const {
  auto res = std::vector<gui::Target>();
  for (auto& target_ptr_ : targets_) {
    res.push_back(*target_ptr_);
  }

  return res;
}

// ----------------------    Hill methods    ----------------------

void DataManager::Add(gui::Hill* h) {
  hills_.emplace_back(h);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::initializer_list<gui::Hill*> new_hills) {
  for (const auto& hill : new_hills) Add(hill);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(lib::Hill data) {
  hills_.emplace_back(new gui::Hill(data));

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::initializer_list<lib::Hill> new_hills) {
  for (const auto& hill : new_hills) Add(hill);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::vector<gui::Hill*> new_hills) {
  for (const auto& hill : new_hills) Add(hill);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::vector<lib::Hill> new_hills) {
  for (const auto& hill : new_hills) Add(hill);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Set(std::initializer_list<gui::Hill*> hills) {
  hills_.clear();
  Add(hills);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::initializer_list<lib::Hill> hills) {
  hills_.clear();
  Add(hills);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::vector<gui::Hill*> hills) {
  hills_.clear();
  Add(hills);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::vector<lib::Hill> hills) {
  hills_.clear();
  Add(hills);

  CheckErrorValues();
  RemoveAllDuplicates();
}

std::vector<gui::Hill*> DataManager::GetHillsPtrs() {
  auto res = std::vector<gui::Hill*>();
  for (auto& hill_ptr_ : hills_) {
    res.push_back(hill_ptr_.get());
  }

  return res;
}

std::vector<gui::Hill> DataManager::GetHills() const {
  auto res = std::vector<gui::Hill>();
  for (auto& hill_ptr_ : hills_) {
    res.push_back(*hill_ptr_);
  }

  return res;
}

// ---------------------- TrappyCircle methods ----------------------s

void DataManager::Add(gui::TrappyCircle* tr_c) {
  tr_circles_.emplace_back(tr_c);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(
    std::initializer_list<gui::TrappyCircle*> new_tr_circles) {
  for (const auto& tr_circle : new_tr_circles) Add(tr_circle);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(lib::TrappyCircle data) {
  tr_circles_.emplace_back(new gui::TrappyCircle(data));

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::initializer_list<lib::TrappyCircle> new_tr_circles) {
  for (const auto& tr_circle : new_tr_circles) Add(tr_circle);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::vector<gui::TrappyCircle*> new_tr_circles) {
  for (const auto& tr_circle : new_tr_circles) Add(tr_circle);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::vector<lib::TrappyCircle> new_tr_circles) {
  for (const auto& tr_circle : new_tr_circles) Add(tr_circle);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Set(std::initializer_list<gui::TrappyCircle*> tr_circles) {
  tr_circles_.clear();
  Add(tr_circles);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::initializer_list<lib::TrappyCircle> tr_circles) {
  tr_circles_.clear();
  Add(tr_circles);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::vector<gui::TrappyCircle*> tr_circles) {
  tr_circles_.clear();
  Add(tr_circles);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::vector<lib::TrappyCircle> tr_circles) {
  tr_circles_.clear();
  Add(tr_circles);

  CheckErrorValues();
  RemoveAllDuplicates();
}

std::vector<gui::TrappyCircle*> DataManager::GetTrappyCirclesPtrs() {
  auto res = std::vector<gui::TrappyCircle*>();
  for (auto& tr_circle_ptr : tr_circles_) {
    res.push_back(tr_circle_ptr.get());
  }

  return res;
}

std::vector<gui::TrappyCircle> DataManager::GetTrappyCircles() const {
  auto res = std::vector<gui::TrappyCircle>();
  for (auto& tr_circle_ptr : tr_circles_) {
    res.push_back(*tr_circle_ptr);
  }

  return res;
}

// ----------------------  TrappyLine methods  ----------------------

void DataManager::Add(gui::TrappyLine* tr_l) {
  tr_lines_.emplace_back(tr_l);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::initializer_list<gui::TrappyLine*> new_tr_lines) {
  for (const auto& tr_line : new_tr_lines) Add(tr_line);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(lib::TrappyLine data) {
  tr_lines_.emplace_back(new gui::TrappyLine(data));

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::initializer_list<lib::TrappyLine> new_tr_lines) {
  for (const auto& tr_line : new_tr_lines) Add(tr_line);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::vector<gui::TrappyLine*> new_tr_lines) {
  for (const auto& tr_line : new_tr_lines) Add(tr_line);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Add(std::vector<lib::TrappyLine> new_tr_lines) {
  for (const auto& tr_line : new_tr_lines) Add(tr_line);

  CheckErrorValues();
  RemoveLastDuplicate();
}

void DataManager::Set(std::initializer_list<gui::TrappyLine*> tr_lines) {
  tr_lines_.clear();
  Add(tr_lines);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::initializer_list<lib::TrappyLine> tr_lines) {
  tr_lines_.clear();
  Add(tr_lines);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::vector<gui::TrappyLine*> tr_lines) {
  tr_lines_.clear();
  Add(tr_lines);

  CheckErrorValues();
  RemoveAllDuplicates();
}

void DataManager::Set(std::vector<lib::TrappyLine> tr_lines) {
  tr_lines_.clear();
  Add(tr_lines);

  CheckErrorValues();
  RemoveAllDuplicates();
}

std::vector<gui::TrappyLine*> DataManager::GetTrappyLinesPtrs() {
  auto res = std::vector<gui::TrappyLine*>();
  for (auto& tr_line_ptr : tr_lines_) {
    res.push_back(tr_line_ptr.get());
  }

  return res;
}

std::vector<gui::TrappyLine> DataManager::GetTrappyLines() const {
  auto res = std::vector<gui::TrappyLine>();
  for (auto& tr_line_ptr : tr_lines_) {
    res.push_back(*tr_line_ptr);
  }

  return res;
}

void DataManager::CheckErrorValues() {
  if (targets_.size() > 10000 || hills_.size() > 10000 ||
      tr_circles_.size() > 10000 || tr_lines_.size() > 10000)
    throw std::invalid_argument("Exceeding the maximum objects number!");
}

bool DataManager::RemoveLastDuplicate() {
  bool res = false;

  if (!targets_.empty())
    for (size_t i = 0; i < targets_.size() - 1; i++)
      if (*targets_[i] == *targets_[targets_.size() - 1]) {
        Remove(gui::ObjectType::Targets, targets_.size() - 1);
        res = true;
      }

  if (!hills_.empty())
    for (size_t i = 0; i < hills_.size() - 1; i++)
      if (*hills_[i] == *hills_[hills_.size() - 1]) {
        Remove(gui::ObjectType::Hills, hills_.size() - 1);
        res = true;
      }

  if (!tr_circles_.empty())
    for (size_t i = 0; i < tr_circles_.size() - 1; i++)
      if (*tr_circles_[i] == *tr_circles_[tr_circles_.size() - 1]) {
        Remove(gui::ObjectType::TrappyCircles, tr_circles_.size() - 1);
        res = true;
      }

  if (!tr_lines_.empty())
    for (size_t i = 0; i < tr_lines_.size() - 1; i++)
      if (*tr_lines_[i] == *tr_lines_[tr_lines_.size() - 1]) {
        Remove(gui::ObjectType::TrappyLines, tr_lines_.size() - 1);
        res = true;
      }

  return res;
}

bool DataManager::RemoveAllDuplicates() {
  bool res = false;

  if (!targets_.empty())
    for (size_t i = 0; i < targets_.size() - 1; i++)
      for (size_t j = i + 1; j < targets_.size(); j++)
        if (*targets_[i] == *targets_[j]) {
          Remove(gui::ObjectType::Targets, j);
          res = true;
        }

  if (!hills_.empty())
    for (size_t i = 0; i < hills_.size() - 1; i++)
      for (size_t j = i + 1; j < hills_.size(); j++)
        if (*hills_[i] == *hills_[j]) {
          Remove(gui::ObjectType::Hills, j);
          res = true;
        }

  if (!tr_circles_.empty())
    for (size_t i = 0; i < tr_circles_.size() - 1; i++)
      for (size_t j = i + 1; j < tr_circles_.size(); j++)
        if (*tr_circles_[i] == *tr_circles_[j]) {
          Remove(gui::ObjectType::TrappyCircles, j);
          res = true;
        }

  if (!tr_lines_.empty())
    for (size_t i = 0; i < tr_lines_.size() - 1; i++)
      for (size_t j = i + 1; j < tr_lines_.size(); j++)
        if (*tr_lines_[i] == *tr_lines_[j]) {
          Remove(gui::ObjectType::TrappyLines, j);
          res = true;
        }

  return res;
}

unsigned short DataManager::GetMinId(gui::ObjectType obj_type) {
  std::vector<unsigned short> ids;
  switch (obj_type) {
    case gui::ObjectType::Targets: {
      for (auto& t : targets_) ids.push_back(t->GetData().GetId());
      unsigned short id = 10000;
      while (!(ids.empty() || find(ids.begin(), ids.end(), id) == ids.end()))
        id++;

      return id;
    }
    case gui::ObjectType::TrappyCircles: {
      for (auto& trc : tr_circles_) ids.push_back(trc->GetData().GetId());
      unsigned short id = 20000;
      while (!(ids.empty() || find(ids.begin(), ids.end(), id) == ids.end()))
        id++;

      return id;
    }
    case gui::ObjectType::TrappyLines: {
      for (auto& trl : tr_lines_) ids.push_back(trl->GetData().GetId());
      unsigned short id = 30000;
      while (!(ids.empty() || find(ids.begin(), ids.end(), id) == ids.end()))
        id++;

      return id;
    }
    case gui::ObjectType::Hills: {
      for (auto& h : hills_) ids.push_back(h->GetData().GetId());
      unsigned short id = 40000;
      while (!(ids.empty() || find(ids.begin(), ids.end(), id) == ids.end()))
        id++;

      return id;
    }
  }
}

}  // namespace data_tools
