#include "JointNode.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

//---------------------------------------------------------------------------------------
JointNode::JointNode(const std::string& name)
	: SceneNode(name)
{
	m_nodeType = NodeType::JointNode;
	rotateTransX = glm::rotate(0.0f, glm::vec3(1,0,0));
	rotateTransY = glm::rotate(0.0f, glm::vec3(0,1,0));
}

//---------------------------------------------------------------------------------------
JointNode::~JointNode() {

}

//---------------------------------------------------------------------------------------
glm::mat4 JointNode::getJointTransform() const {
	return trans * rotateTransY * rotateTransX;
}

void JointNode::rotateJoint(char axis, float angle) {
	// Set up which axis to use
	switch(axis) {
		case 'x':
			m_curAngleX = glm::clamp(m_curAngleX + angle, m_joint_x.min, m_joint_x.max);
			rotateTransX = glm::rotate(glm::radians((float) m_curAngleX), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case 'y':
			m_curAngleY = glm::clamp(m_curAngleY + angle, m_joint_y.min, m_joint_y.max);
			rotateTransY = glm::rotate(glm::radians((float) m_curAngleY), glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		default:
			return;
	}
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_x(double min, double init, double max) {
	m_joint_x.min = min;
	m_joint_x.init = init;
	m_joint_x.max = max;
	m_curAngleX = init;
	rotateTransX = glm::rotate(glm::radians((float) m_curAngleX), glm::vec3(0.0f, 1.0f, 0.0f));
}

//---------------------------------------------------------------------------------------
void JointNode::set_joint_y(double min, double init, double max) {
	m_joint_y.min = min;
	m_joint_y.init = init;
	m_joint_y.max = max;
	m_curAngleY = init;
	rotateTransY = glm::rotate(glm::radians((float) m_curAngleY), glm::vec3(0.0f, 0.0f, 1.0f));
}
