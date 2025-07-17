// header file:
#include "hill.h"

// std libs:
#include <stdexcept>

namespace lib {

QJsonObject Hill::GetJsonInfo() const {
  QVariantMap hill_map;
  hill_map.insert("Id", GetId());

  QJsonArray vertices_array;
  for (size_t i = 0; i < vertices_.size(); i++) {
    QJsonObject v_obj;
    v_obj.insert("X", vertices_[i].x);
    v_obj.insert("Y", vertices_[i].y);
    vertices_array.append(v_obj);
  }
  hill_map.insert("Vertices", vertices_array);

  return QJsonObject::fromVariantMap(hill_map);
}

void Hill::SetJsonInfo(const QJsonObject& hill_obj) {
  if (!(hill_obj.contains("Vertices") && hill_obj.contains("Id")))
    throw std::invalid_argument(
        "Invalid file format: missing Vertices or Id field in Hills!");

  QJsonArray vertices_array = hill_obj.value("Vertices").toArray();
  for (size_t i = 0; i < static_cast<size_t>(vertices_array.size()); i++) {
    lib::Point vertice;
    QJsonObject v_obj = vertices_array[i].toObject();
    if (!(v_obj.contains("X") && v_obj.contains("Y")))
      throw std::invalid_argument(
          "Invalid file format: missing X or Y field in Hills!");

    vertice.x = v_obj.value("X").toDouble();
    vertice.y = v_obj.value("Y").toDouble();
    vertices_.push_back(vertice);
  }

  unsigned short id = static_cast<unsigned short>(hill_obj.value("Id").toInt());
  if (id < 40000 || id > 49999)
    throw std::invalid_argument("Invalid file format: incorrect id in 'Hill'!");
  SetId(id);

  CheckErrorValues_();
}

bool Hill::IsChanged(const QJsonObject& hill_obj) const {
  QJsonArray json_vertices = hill_obj.value("Vertices").toArray();
  for (size_t i = 0; i < vertices_.size() - 1; i++) {
    QJsonObject v_obj = json_vertices[i].toObject();
    lib::Point p = {v_obj.value("X").toDouble(), v_obj.value("Y").toDouble()};
    if (p != vertices_[i]) return true;
  }
  return false;
}

bool Hill::operator==(const Hill& hill) const {
  if (vertices_.size() != hill.GetVertices().size()) return false;

  for (size_t i = 0; i < vertices_.size(); i++)
    if (vertices_[i] != hill.GetVertices()[i]) return false;

  return true;
}

void Hill::CheckErrorValues_() const {
  for (const auto& vert : vertices_)
    if (vert.x > max_coord || vert.y > max_coord)
      throw std::invalid_argument("Exceeding the maximum permissible values!");
}

}  // namespace lib
