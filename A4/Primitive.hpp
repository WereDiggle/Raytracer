#pragma once

#include <glm/glm.hpp>
#include "Ray.hpp"
#include "Intersect.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual Intersect checkIntersection(const Ray & ray);
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
};

class Cube : public Primitive {
public:
  virtual ~Cube();
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual Intersect checkIntersection(const Ray & ray);

private:
  glm::vec3 m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size), m_dimensions(glm::vec3((float) size))
  {
  }

  NonhierBox(const glm::vec3& pos, const glm::vec3& dimensions)
    : m_pos(pos), m_size(0), m_dimensions(dimensions)
  {
  }
  
  virtual ~NonhierBox();
  virtual Intersect checkIntersection(const Ray & ray);

private:
  glm::vec3 m_pos;
  glm::vec3 m_dimensions;
  double m_size;
};
