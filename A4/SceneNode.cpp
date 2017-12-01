#include "SceneNode.hpp"

#include "cs488-framework/MathUtils.hpp"

#include <iostream>
#include <sstream>
using namespace std;

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;


// Static class variable
unsigned int SceneNode::nodeInstanceCount = 0;


//---------------------------------------------------------------------------------------
SceneNode::SceneNode(const std::string& name)
  : m_name(name),
	m_nodeType(NodeType::SceneNode),
	trans(mat4()),
	invtrans(mat4()),
	m_nodeId(nodeInstanceCount++)
{

}

//---------------------------------------------------------------------------------------
// Deep copy
SceneNode::SceneNode(const SceneNode & other)
	: m_nodeType(other.m_nodeType),
	  m_name(other.m_name),
	  trans(other.trans),
	  invtrans(other.invtrans)
{
	for(SceneNode * child : other.children) {
		this->children.push_front(new SceneNode(*child));
	}
}

//---------------------------------------------------------------------------------------
SceneNode::~SceneNode() {
	for(SceneNode * child : children) {
		delete child;
	}
}

//---------------------------------------------------------------------------------------
Intersect SceneNode::checkIntersection(const Ray & ray) {
	return Intersect();
}

Intersect SceneNode::checkDoubleSidedIntersection(const Ray & ray) {
	return Intersect();
}

Intersect SceneNode::castRay(const Ray & ray) {

	Ray transformedRay = ray.transformRay(invtrans);

	// remember that checkIntersection is virtual, so it should call the derived checkIntersection if possible
	Intersect rayIntersect = checkIntersection(transformedRay);

	// recurse through all children, casting transformed rays at all of them
	for (SceneNode * childNode : children) {
		Intersect curRayIntersect = childNode->castRay(transformedRay);
		if (curRayIntersect.isHit) {
			if (!rayIntersect.isHit || curRayIntersect.distanceHit < rayIntersect.distanceHit) {
				rayIntersect = curRayIntersect;
			}
		}
	}

	return rayIntersect.transformIntersect(trans);
}

Intersect SceneNode::castShadowRay(const Ray & ray, const glm::vec3 & light) {

	Ray lightRay = Ray(light, ray.origin);
	lightRay = lightRay.transformRay(invtrans);
	Ray transformedRay = ray.transformRay(invtrans);

	double distanceToLight = glm::distance(transformedRay.origin, lightRay.origin);

	// remember that checkIntersection is virtual, so it should call the derived checkIntersection if possible
	Intersect rayIntersect = checkIntersection(transformedRay);

	double totalTransparency = 1.0;
	if (rayIntersect.isHit && rayIntersect.distanceHit <= distanceToLight) {
		totalTransparency -= (1-rayIntersect.transparency);
	}

	// recurse through all children, casting transformed rays at all of them
	// We want to sum up all transparency values between the shadow and the light
	for (SceneNode * childNode : children) {
		Intersect curRayIntersect = childNode->castShadowRay(transformedRay, lightRay.origin);
		if (curRayIntersect.isHit && curRayIntersect.distanceHit <= distanceToLight) {
			totalTransparency -= (1-curRayIntersect.transparency);
			rayIntersect = curRayIntersect;
		}
	}

	rayIntersect.transparency = totalTransparency;

	return rayIntersect.transformIntersect(trans);
}

Intersect SceneNode::castDoubleSidedRay(const Ray & ray) {

	Ray transformedRay = ray.transformRay(invtrans);

	// remember that checkIntersection is virtual, so it should call the derived checkIntersection if possible
	Intersect rayIntersect = checkDoubleSidedIntersection(transformedRay);

	// recurse through all children, casting transformed rays at all of them
	for (SceneNode * childNode : children) {
		Intersect curRayIntersect = childNode->castDoubleSidedRay(transformedRay);
		if (curRayIntersect.isHit) {
			if (!rayIntersect.isHit || curRayIntersect.distanceHit < rayIntersect.distanceHit) {
				rayIntersect = curRayIntersect;
			}
		}
	}

	return rayIntersect.transformIntersect(trans);

}

Intersect SceneNode::castOnceRay(const Ray & ray) {
	Ray transformedRay = ray.transformRay(invtrans);

	// remember that checkIntersection is virtual, so it should call the derived checkIntersection if possible
	Intersect rayIntersect = checkIntersection(transformedRay);

	if (!rayIntersect.isHit) {

		// recurse through all children, casting transformed rays at all of them
		for (SceneNode * childNode : children) {
			Intersect curRayIntersect = childNode->castRay(transformedRay);
			if (curRayIntersect.isHit) {
				rayIntersect = curRayIntersect;
				break;
			}
		}
	}

	return rayIntersect.transformIntersect(trans);
}

//---------------------------------------------------------------------------------------
void SceneNode::set_transform(const glm::mat4& m) {
	trans = m;
	invtrans = glm::inverse(m);
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_transform() const {
	return trans;
}

//---------------------------------------------------------------------------------------
const glm::mat4& SceneNode::get_inverse() const {
	return invtrans;
}

//---------------------------------------------------------------------------------------
void SceneNode::add_child(SceneNode* child) {
	children.push_back(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::remove_child(SceneNode* child) {
	children.remove(child);
}

//---------------------------------------------------------------------------------------
void SceneNode::rotate(char axis, float angle) {
	vec3 rot_axis;

	switch (axis) {
		case 'x':
			rot_axis = vec3(1,0,0);
			break;
		case 'y':
			rot_axis = vec3(0,1,0);
	        break;
		case 'z':
			rot_axis = vec3(0,0,1);
	        break;
		default:
			break;
	}
	mat4 rot_matrix = glm::rotate(degreesToRadians(angle), rot_axis);
	set_transform( rot_matrix * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::scale(const glm::vec3 & amount) {
	set_transform( glm::scale(amount) * trans );
}

//---------------------------------------------------------------------------------------
void SceneNode::translate(const glm::vec3& amount) {
	set_transform( glm::translate(amount) * trans );
}


//---------------------------------------------------------------------------------------
int SceneNode::totalSceneNodes() const {
	return nodeInstanceCount;
}

//---------------------------------------------------------------------------------------
std::ostream & operator << (std::ostream & os, const SceneNode & node) {

	//os << "SceneNode:[NodeType: ___, name: ____, id: ____, isSelected: ____, transform: ____"
	switch (node.m_nodeType) {
		case NodeType::SceneNode:
			os << "SceneNode";
			break;
		case NodeType::GeometryNode:
			os << "GeometryNode";
			break;
		case NodeType::JointNode:
			os << "JointNode";
			break;
	}
	os << ":[";

	os << "name:" << node.m_name << ", ";
	os << "id:" << node.m_nodeId;

	os << "]\n";
	return os;
}
