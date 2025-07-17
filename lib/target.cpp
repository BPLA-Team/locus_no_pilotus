// header file
#include "target.h"

namespace lib {

QJsonObject Target::GetJsonInfo() const {
  QVariantMap target_map;

  target_map.insert("Id", GetId());
  target_map.insert("X", p_.x);
  target_map.insert("Y", p_.y);

  return QJsonObject::fromVariantMap(target_map);
}

void Target::SetJsonInfo(const QJsonObject& target_obj) {
  if (!(target_obj.contains("X") && target_obj.contains("Y") &&
        target_obj.contains("Id")))
    throw std::invalid_argument(
        "Invalid file format: missing X, Y or Id field in Targets!");

  double x = target_obj.value("X").toDouble();
  double y = target_obj.value("Y").toDouble();
  SetPoint(x, y);

  unsigned short id =
      static_cast<unsigned short>(target_obj.value("Id").toInt());
  if (id < 10000 || id > 19999)
    throw std::invalid_argument(
        "Invalid file format: incorrect id in 'Target'!");
  SetId(id);

  CheckErrorValues_();
}

bool Target::IsChanged(const QJsonObject& target_obj) const {
  Point p = {target_obj.value("X").toDouble(),
             target_obj.value("Y").toDouble()};
  return p != p_;
}

bool Target::operator==(const Target& target) const {
  return p_ == target.GetPoint();
}

void Target::CheckErrorValues_() const {
  if (p_.x > max_coord || p_.y > max_coord)
    throw std::invalid_argument("Exceeding the maximum permissible values!");
}

}  // namespace lib
