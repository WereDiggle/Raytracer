#pragma once

#include "Material.hpp"
#include "Ray.hpp"
#include "Intersect.hpp"

#include <glm/glm.hpp>

#include <list>
#include <string>
#include <iostream>

enum class NodeType {
	SceneNode,
	GeometryNode,
	JointNode
};

class SceneNode {
public:
    SceneNode(const std::string & name);

	SceneNode(const SceneNode & other);

    virtual ~SceneNode();

    // Cast a ray, returning the closest intersection where the normal of the surface faces opposite the ray direction
    virtual Intersect castRay(const Ray & ray);

    // Cast a ray for shadow calculations, Sums up the non-transparency of every object on the way to the light
    virtual Intersect castShadowRay(const Ray & ray, const glm::vec3 & light);

    // Cast a ray, returning if it hits any object along the path, not necessarily the closest. 
    virtual Intersect castOnceRay(const Ray & ray);

    // Cast a ray, returning the closest object it hits, regardless of normal facing.
    virtual Intersect castDoubleSidedRay(const Ray & ray);

    // Check if it hits the object, regardless of normal facing
    virtual Intersect checkDoubleSidedIntersection(const Ray & ray);

    // Check if it hits the object, normal facing away
    virtual Intersect checkIntersection(const Ray & ray);
    
	int totalSceneNodes() const;
    
    const glm::mat4& get_transform() const;
    const glm::mat4& get_inverse() const;
    
    void set_transform(const glm::mat4& m);
    
    void add_child(SceneNode* child);
    
    void remove_child(SceneNode* child);

	//-- Transformations:
    void rotate(char axis, float angle);
    void scale(const glm::vec3& amount);
    void translate(const glm::vec3& amount);


	friend std::ostream & operator << (std::ostream & os, const SceneNode & node);

    // Transformations
    glm::mat4 trans;
    glm::mat4 invtrans;
    
    std::list<SceneNode*> children;

	NodeType m_nodeType;
	std::string m_name;
	unsigned int m_nodeId;

private:
	// The number of SceneNode instances.
	static unsigned int nodeInstanceCount;
};
