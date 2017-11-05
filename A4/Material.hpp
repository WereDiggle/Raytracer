#pragma once

#include <glm/glm.hpp>

class Material {
public:
  virtual ~Material();
  virtual glm::vec3 getColour() = 0;

protected:
  Material();
};
