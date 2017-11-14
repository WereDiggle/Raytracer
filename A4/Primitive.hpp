#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

#include "Ray.hpp"
#include "Intersect.hpp"

class Primitive {
protected:
  static const glm::vec3 axisNormals[6];
public:
  virtual ~Primitive();
  virtual Intersect checkIntersection(const Ray & ray);
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  virtual Intersect checkIntersection(const Ray & ray);
};

class Cube : public Primitive {
public:
  Cube() {
    points[0] = glm::vec3(1,1,1); 
    points[1] = glm::vec3(0,0,0);
  }
  virtual ~Cube();
  virtual Intersect checkIntersection(const Ray & ray);
private:
  glm::vec3 points[2];
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
    points[0] = m_pos+glm::vec3(size);
    points[1] = m_pos;
  }

  NonhierBox(const glm::vec3& pos, const glm::vec3& dimensions)
    : m_pos(pos), m_size(0), m_dimensions(dimensions)
  {
    points[0] = m_pos + m_dimensions;
    points[1] = m_pos;
  }
  
  virtual ~NonhierBox();
  virtual Intersect checkIntersection(const Ray & ray);

private:
  glm::vec3 points[2];
  glm::vec3 m_pos;
  glm::vec3 m_dimensions;
  double m_size;
};
