#pragma once

// std libs:
#include <utility>  // for pair

// our code libs:
#include "base.h"
#include "target.h"

namespace lib {

/// @brief Линия опасного перелета
class TrappyLine : public JSONable {
 public:
  TrappyLine() = default;

  TrappyLine(Target* first_target, Target* second_target)
      : targets_{std::make_pair(first_target, second_target)} {}
  TrappyLine(std::pair<Target*, Target*> targets) : targets_{targets} {}

  TrappyLine(const TrappyLine&) = default;
  TrappyLine(TrappyLine&&) = default;

  TrappyLine& operator=(const TrappyLine&) = default;
  TrappyLine& operator=(TrappyLine&&) = default;

  QJsonObject GetJsonInfo() const override;
  void SetJsonInfo(const QJsonObject&) override {}
  bool IsChanged(const QJsonObject& trappy_line_obj) const override;

  void SetTargets(Target* first_target, Target* second_target) {
    targets_ = std::make_pair(first_target, second_target);
  }
  void SetTargets(std::pair<Target*, Target*> targets) { targets_ = targets; }

  std::pair<Target, Target> GetTargets() const;

  std::pair<Target*, Target*>& GetTargetsPtrs() { return targets_; }
  const std::pair<Target*, Target*>& GetTargetsPtrs() const { return targets_; }

  bool operator==(const TrappyLine&) const;

 private:
  /// @throw td::runtime_error: класс не содержит проверяемых данных
  void CheckErrorValues_() const override {
    throw std::runtime_error("dev: no checkable error values in TrappyLines");
  }

  // здесь нет смысла использовать умные указатели, так как мы не создаём новых
  std::pair<Target*, Target*> targets_;
};

}  // namespace lib
