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
    : m_pos(pos), m_dimension(glm::dvec3(size))
  {
    glm::vec3 points[2] = {
        m_pos+m_dimension, m_pos,
    };
  }

  NonhierBox(const glm::vec3& pos, const glm::vec3& dimension)
    : m_pos(pos), m_dimension(dimension)
  {
    glm::vec3 points[2] = {
        m_pos+m_dimension, m_pos,
    };
  }
  
  virtual ~NonhierBox();
  virtual Intersect checkIntersection(const Ray & ray);

private:
  static const glm::dvec3 normals[6];

  glm::dvec3 m_pos;
  glm::dvec3 m_dimension;
  glm::dvec3 points[2];
};
