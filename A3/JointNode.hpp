#pragma once

#include "SceneNode.hpp"

class JointNode : public SceneNode {
public:
	JointNode(const std::string & name);
	virtual ~JointNode();

	glm::mat4 getJointTransform() const;
	void rotateJoint(char axis, float angle);

	void set_joint_x(double min, double init, double max);
	void set_joint_y(double min, double init, double max);

	struct JointRange {
		double min, init, max;
	};

	double m_curAngleX;
	double m_curAngleY;

	// rotation transformations specific to joints
	glm::mat4 rotateTrans;
	glm::mat4 rotateTransX;
	glm::mat4 rotateTransY;

	JointRange m_joint_x, m_joint_y;
};
