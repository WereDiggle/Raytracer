#pragma once

#include <glm/glm.hpp>

#include "Texture.hpp"
#include "Material.hpp"

class PhongMaterial : public Material {

public:

  PhongMaterial(const glm::vec3& kd, const glm::vec3& ks, double shininess);

  glm::vec3 getColour();

  double getTransparency();

  glm::vec3 BRDF(const glm::vec3 & surfaceNormal, const glm::vec3 & lightDirection, const glm::vec3 & viewDirection);

  glm::vec3 getLighting(const glm::vec3 & surfaceNormal, 
                        const glm::vec3 & lightDirection, const glm::vec3 & lightIntensity, double lightDistance, double * lightFalloff, 
                        const glm::vec3 & viewDirection,
                        double textureU, double textureV, Texture * bitmap, Texture * bumpmap);
  virtual ~PhongMaterial();

private:
  glm::vec3 m_kd;
  glm::vec3 m_ks;

  double m_shininess;
};
