#pragma once

#include <glm/glm.hpp>

class Material {
public:
  virtual ~Material();
  virtual glm::vec3 getColour() = 0;
  virtual glm::vec3 getLighting(const glm::vec3 & surfaceNormal, const glm::vec3 & lightDirection, const glm::vec3 & lightIntensity, double lightDistance, double * lightFalloff, const glm::vec3 & viewDirection) = 0;

protected:
  Material();
};
