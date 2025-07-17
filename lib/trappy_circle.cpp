// header file:
#include "trappy_circle.h"

// std libs:
#include <stdexcept>

namespace lib {

TrappyCircle::TrappyCircle(Point center, double radius)
    : center_{center}, radius_{radius} {
  if (radius < 0)
    throw std::invalid_argument(
        "TrappyCircles cannot have of negative radius!");

  CheckErrorValues_();
}

QJsonObject TrappyCircle::GetJsonInfo() const {
  QVariantMap trappy_circle_map;

  trappy_circle_map.insert("Id", GetId());
  trappy_circle_map.insert("X", center_.x);
  trappy_circle_map.insert("Y", center_.y);
  trappy_circle_map.insert("Radius", radius_);

  return QJsonObject::fromVariantMap(trappy_circle_map);
}

void TrappyCircle::SetJsonInfo(const QJsonObject& trappy_circle_obj) {
  if (!(trappy_circle_obj.contains("X") && trappy_circle_obj.contains("Y") &&
        trappy_circle_obj.contains("Radius") &&
        trappy_circle_obj.contains("Id")))
    throw std::invalid_argument(
        "Invalid file format: missing X,Y,Radius or Id field in "
        "TrappyCircles!");

  double x = trappy_circle_obj.value("X").toDouble();
  double y = trappy_circle_obj.value("Y").toDouble();
  double r = trappy_circle_obj.value("Radius").toDouble();

  SetCenter({x, y});
  SetRadius(r);

  unsigned short id =
      static_cast<unsigned short>(trappy_circle_obj.value("Id").toInt());
  if (id < 20000 || id > 29999)
    throw std::invalid_argument(
        "Invalid file format: incorrect id in 'Trappy_Circle'!");
  SetId(id);

  CheckErrorValues_();
}

bool TrappyCircle::IsChanged(const QJsonObject& trappy_circle_obj) const {
  Point c = {trappy_circle_obj.value("X").toDouble(),
             trappy_circle_obj.value("Y").toDouble()};
  double r = trappy_circle_obj.value("Radius").toDouble();
  return c != center_ || abs(r - radius_) > 10.E-7;
}

bool TrappyCircle::operator==(const TrappyCircle& tr_circle) const {
  return center_ == tr_circle.GetCenter() &&
         abs(radius_ - tr_circle.GetRadius()) < 10.E-7;
}

void TrappyCircle::CheckErrorValues_() const {
  if (center_.x > max_coord || center_.y > max_coord || radius_ > max_coord)
    throw std::invalid_argument("Exceeding the maximum permissible values!");
}

}  // namespace lib
