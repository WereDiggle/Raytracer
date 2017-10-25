#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"
#include "cs488-framework/MeshConsolidator.hpp"

#include "SceneNode.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <vector>

class JointNode;

struct LightSource {
	glm::vec3 position;
	glm::vec3 rgbIntensity;
};

enum MouseMode { Model = 0, Joint};

class A3 : public CS488Window {
public:
	A3(const std::string & luaSceneFile);
	virtual ~A3();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	//-- Virtual callback methods
	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	//-- One time initialization methods:
	void processLuaSceneFile(const std::string & filename);
	void createShaderProgram();
	void enableVertexShaderInputSlots();
	void uploadVertexDataToVbos(const MeshConsolidator & meshConsolidator);
	void mapVboDataToVertexShaderInputLocations();
	void initViewMatrix();
	void initLightSources();
	void mapJoints(SceneNode & node);

	void initPerspectiveMatrix();
	void uploadCommonSceneUniforms();
	void renderSceneGraph(const SceneNode &node);
	void renderNode(const SceneNode &node, const glm::mat4 & parentTransMatrix);
	void renderPickingNode(const SceneNode & node, const glm::mat4 & parentTransMatrix, unsigned int jointId = 0);
	void renderArcCircle();
	unsigned int pickJointUnderMouse();
	void highlightJoint(SceneNode & node, bool highlight);
	void highlightNodes(SceneNode & node, bool highlight);

	void rotateAllSelectedJoints(double dx, double dy);

	// Reset Functions
	void resetPosition();
	void resetOrientation();
	void resetJoints();
	void resetAll();

	// helper functions
	glm::vec4 intToColour(unsigned int i);

	glm::mat4 m_perpsective;
	glm::mat4 m_view;

	LightSource m_light;

	// The control mode for mouse inputs
	int curMouseMode = MouseMode::Joint;

	// mouse down variables
	int leftMouseDown = 0;
	int middleMouseDown = 0;
	int rightMouseDown = 0;

	// Mouse position on the previous mouse move event
	double lastMouseX = 0;
	double lastMouseY = 0;

	// test rotation, for getting a better idea of the model
	float testRotation = 0.0f;
	float testDistance = 5.0f;

	//-- GL resources for the picker
	GLint m_picker_positionAttribLocation;
	ShaderProgram m_shader_picker;

	//-- GL resources for mesh geometry data:
	GLuint m_vao_meshData;
	GLuint m_vbo_vertexPositions;
	GLuint m_vbo_vertexNormals;
	GLint m_positionAttribLocation;
	GLint m_normalAttribLocation;
	ShaderProgram m_shader;

	//-- GL resources for trackball circle geometry:
	GLuint m_vbo_arcCircle;
	GLuint m_vao_arcCircle;
	GLint m_arc_positionAttribLocation;
	ShaderProgram m_shader_arcCircle;

	// BatchInfoMap is an associative container that maps a unique MeshId to a BatchInfo
	// object. Each BatchInfo object contains an index offset and the number of indices
	// required to render the mesh with identifier MeshId.
	BatchInfoMap m_batchInfoMap;

	std::string m_luaSceneFile;

	std::shared_ptr<SceneNode> m_rootNode;

	// Extra info for the node heirarchy

	// maps joint node IDs to node pointers
	std::map<unsigned int, JointNode* > m_jointMap; 

	// undo/redo stack
	// each element in the stack is a map from node id to X and Y joint angles
	std::vector<std::map<unsigned int, std::pair<double, double> > > m_undoStack;
	std::vector<std::map<unsigned int, std::pair<double, double> > > m_redoStack;
	int m_curUndo;

	void pushCurrentJointState();
	void undo();
	void redo();
};
