// header file:
#include "data_manager.h"

namespace data_tools {

// MARK: macros hell

#define CHECK_AND_REMOVE_LAST \
  {                           \
    CheckErrorValues_();      \
    RemoveLastDuplicate();    \
  }

#define CHECK_AND_REMOVE_ALL \
  {                          \
    CheckErrorValues_();     \
    RemoveAllDuplicates();   \
  }

#define ADD_BY_LINK(Object, object_name)                 \
  {                                                      \
    object_name##s_.emplace_back(new gui::Object(data)); \
                                                         \
    CHECK_AND_REMOVE_LAST;                               \
  }

#define ADD_BY_VECTOR(object_name)                                         \
  {                                                                        \
    for (const auto& object_name : new_##object_name##s) Add(object_name); \
                                                                           \
    CHECK_AND_REMOVE_LAST;                                                 \
  }

#define SET_BY_VECTOR(object_name) \
  {                                \
    object_name##s_.clear();       \
    Add(object_name##s);           \
                                   \
    CHECK_AND_REMOVE_ALL;          \
  }

#define GET_OBJECTS(Object, object_name)                         \
  std::vector<gui::Object*> DataManager::Get##Object##sPtrs() {  \
    auto res = std::vector<gui::Object*>();                      \
    for (auto& object_name##_ptr_ : object_name##s_)             \
      res.push_back(object_name##_ptr_.get());                   \
                                                                 \
    return res;                                                  \
  }                                                              \
                                                                 \
  std::vector<gui::Object> DataManager::Get##Object##s() const { \
    auto res = std::vector<gui::Object>();                       \
    for (auto& object_name##_ptr_ : object_name##s_)             \
      res.push_back(*object_name##_ptr_);                        \
                                                                 \
    return res;                                                  \
  }

#define ADD_BY_GUI_POINTER(Object, object_name)                                \
  void DataManager::Add(gui::Object* object_name##_ptr) {                      \
    object_name##s_.emplace_back(object_name##_ptr);                           \
    object_name##_ptr->GetData().SetId(GetMinId_(gui::ObjectType::Object##s)); \
                                                                               \
    CHECK_AND_REMOVE_LAST;                                                     \
  }                                                                            \
                                                                               \
  void DataManager::Add(std::vector<gui::Object*> new_##object_name##s) {      \
    for (const auto& object_name : new_##object_name##s) {                     \
      Add(object_name);                                                        \
      object_name->GetData().SetId(GetMinId_(gui::ObjectType::Object##s));     \
    }                                                                          \
                                                                               \
    CHECK_AND_REMOVE_LAST;                                                     \
  }

#define OBJECT_METHODS(Object, object_name)                               \
  void DataManager::Add(lib::Object& data) {                              \
    ADD_BY_LINK(Object, object_name);                                     \
  }                                                                       \
                                                                          \
  void DataManager::Add(const lib::Object& data) {                        \
    ADD_BY_LINK(Object, object_name);                                     \
  }                                                                       \
                                                                          \
  ADD_BY_GUI_POINTER(Object, object_name)                                 \
                                                                          \
  void DataManager::Add(std::vector<lib::Object>& new_##object_name##s) { \
    ADD_BY_VECTOR(object_name);                                           \
  }                                                                       \
                                                                          \
  void DataManager::Add(                                                  \
      const std::vector<lib::Object>& new_##object_name##s) {             \
    ADD_BY_VECTOR(object_name);                                           \
  }                                                                       \
                                                                          \
  void DataManager::Set(std::vector<gui::Object*> object_name##s) {       \
    SET_BY_VECTOR(object_name);                                           \
  }                                                                       \
                                                                          \
  void DataManager::Set(std::vector<lib::Object>& object_name##s) {       \
    SET_BY_VECTOR(object_name);                                           \
  }                                                                       \
                                                                          \
  void DataManager::Set(const std::vector<lib::Object>& object_name##s) { \
    SET_BY_VECTOR(object_name);                                           \
  }                                                                       \
                                                                          \
  GET_OBJECTS(Object, object_name)

// MARK: macros end

void DataManager::Remove(gui::ObjectType obj_type, size_t index) {
  switch (obj_type) {
    case gui::ObjectType::Targets: {
      targets_.erase(targets_.begin() + index);

      // если был удалён первый, не явл. последним, меняем следующий на аэропорт
      if (index == 0 && !targets_.empty()) {
        targets_[0].reset(new gui::Airport(targets_[0]->GetData()));
      }

      break;
    }

    case gui::ObjectType::Hills: {
      hills_.erase(hills_.begin() + index);
      break;
    }

    case gui::ObjectType::TrappyCircles: {
      trappy_circles_.erase(trappy_circles_.begin() + index);
      break;
    }

    case gui::ObjectType::TrappyLines: {
      trappy_lines_.erase(trappy_lines_.begin() + index);
      break;
    }
  }

  CheckErrorValues_();
}

void DataManager::Clear() {
  targets_.clear();
  trappy_circles_.clear();
  trappy_lines_.clear();
  hills_.clear();

  CheckErrorValues_();
}

// ----------------------   Target methods   ----------------------

void DataManager::Add(gui::Target* target) {
  if (targets_.empty()) target = new gui::Airport(*target);

  targets_.emplace_back(target);
  target->GetData().SetId(GetMinId_(gui::ObjectType::Targets));

  CHECK_AND_REMOVE_LAST;
}

void DataManager::Add(std::vector<gui::Target*> new_targets) {
  for (const auto& target : new_targets) {
    Add(target);
    target->GetData().SetId(GetMinId_(gui::ObjectType::Targets));
  }

  CHECK_AND_REMOVE_LAST;
}

void DataManager::Add(lib::Target& data) {
  if (targets_.empty())
    targets_.emplace_back(new gui::Airport(data));
  else
    targets_.emplace_back(new gui::Target(data));

  CHECK_AND_REMOVE_LAST;
}

void DataManager::Add(const lib::Target& data) {
  if (targets_.empty())
    targets_.emplace_back(new gui::Airport(data));
  else
    targets_.emplace_back(new gui::Target(data));

  CHECK_AND_REMOVE_LAST;
}

void DataManager::Add(std::vector<lib::Target>& new_targets) {
  ADD_BY_VECTOR(target);
}

void DataManager::Add(const std::vector<lib::Target>& new_targets) {
  ADD_BY_VECTOR(target);
}

void DataManager::Set(std::vector<gui::Target*> targets) {
  SET_BY_VECTOR(target);
}

void DataManager::Set(std::vector<lib::Target>& targets) {
  SET_BY_VECTOR(target);
}

void DataManager::Set(const std::vector<lib::Target>& targets) {
  SET_BY_VECTOR(target);
}

GET_OBJECTS(Target, target)

// ----------------------    Hill methods    ------------------------

OBJECT_METHODS(Hill, hill)

// ---------------------- TrappyCircle methods ----------------------

OBJECT_METHODS(TrappyCircle, trappy_circle)

// ----------------------  TrappyLine methods  ----------------------

OBJECT_METHODS(TrappyLine, trappy_line)

// ------------------------------------------------------------------

void DataManager::CheckErrorValues_() {
  if (targets_.size() > 10000 || hills_.size() > 10000 ||
      trappy_circles_.size() > 10000 || trappy_lines_.size() > 10000)
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

  if (!trappy_circles_.empty())
    for (size_t i = 0; i < trappy_circles_.size() - 1; i++)
      if (*trappy_circles_[i] == *trappy_circles_[trappy_circles_.size() - 1]) {
        Remove(gui::ObjectType::TrappyCircles, trappy_circles_.size() - 1);
        res = true;
      }

  if (!trappy_lines_.empty())
    for (size_t i = 0; i < trappy_lines_.size() - 1; i++)
      if (*trappy_lines_[i] == *trappy_lines_[trappy_lines_.size() - 1]) {
        Remove(gui::ObjectType::TrappyLines, trappy_lines_.size() - 1);
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

  if (!trappy_circles_.empty())
    for (size_t i = 0; i < trappy_circles_.size() - 1; i++)
      for (size_t j = i + 1; j < trappy_circles_.size(); j++)
        if (*trappy_circles_[i] == *trappy_circles_[j]) {
          Remove(gui::ObjectType::TrappyCircles, j);
          res = true;
        }

  if (!trappy_lines_.empty())
    for (size_t i = 0; i < trappy_lines_.size() - 1; i++)
      for (size_t j = i + 1; j < trappy_lines_.size(); j++)
        if (*trappy_lines_[i] == *trappy_lines_[j]) {
          Remove(gui::ObjectType::TrappyLines, j);
          res = true;
        }

  return res;
}

unsigned short DataManager::GetMinId_(gui::ObjectType obj_type) {
  std::vector<unsigned short> ids;

  switch (obj_type) {
    case gui::ObjectType::Targets: {
      for (auto& target : targets_) ids.push_back(target->GetData().GetId());
      unsigned short id = 10000;
      while (!(ids.empty() || find(ids.begin(), ids.end(), id) == ids.end()))
        id++;

      return id;
    }

    case gui::ObjectType::TrappyCircles: {
      for (auto& trappy_circle : trappy_circles_)
        ids.push_back(trappy_circle->GetData().GetId());
      unsigned short id = 20000;
      while (!(ids.empty() || find(ids.begin(), ids.end(), id) == ids.end()))
        id++;

      return id;
    }

    case gui::ObjectType::TrappyLines: {
      for (auto& trappy_line : trappy_lines_)
        ids.push_back(trappy_line->GetData().GetId());
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

  // this case is impossible
  return SHRT_MAX;
}

}  // namespace data_tools
