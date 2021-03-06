#pragma once

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include <glm/glm.hpp>

#include <vector>

// Set a global maximum number of vertices in order to pre-allocate VBO data
// in one shot, rather than reallocating each frame.
const GLsizei kMaxVertices = 1000;


// Convenience class for storing vertex data in CPU memory.
// Data should be copied over to GPU memory via VBO storage before rendering.
class VertexData {
public:
	VertexData();

	std::vector<glm::vec2> positions;
	std::vector<glm::vec3> colours;
	GLuint index;
	GLsizei numVertices;
};

struct Line {
	glm::vec3 A;
	glm::vec3 B;

	// should you draw this or not.
	// Don't want case where a single dot is drawn even though we don't want any of the line to be drawn
	bool valid;

	Line(const glm::vec3 & A, const glm::vec3 & B) : A(A), B(B), valid(true) 
	{
	} 

	Line() : A(glm::vec3(0)), B(glm::vec3(0)), valid(false) 
	{
	} 
};

enum Mode { ModelRotation = 0, ModelTranslation, ModelScale, ViewRotation, ViewTranslation, ViewPort, Perspective};

class A2 : public CS488Window {
public:
	A2();
	virtual ~A2();

protected:
	virtual void init() override;
	virtual void appLogic() override;
	virtual void guiLogic() override;
	virtual void draw() override;
	virtual void cleanup() override;

	virtual bool cursorEnterWindowEvent(int entered) override;
	virtual bool mouseMoveEvent(double xPos, double yPos) override;
	virtual bool mouseButtonInputEvent(int button, int actions, int mods) override;
	virtual bool mouseScrollEvent(double xOffSet, double yOffSet) override;
	virtual bool windowResizeEvent(int width, int height) override;
	virtual bool keyInputEvent(int key, int action, int mods) override;

	void createShaderProgram();
	void enableVertexAttribIndices();
	void generateVertexBuffers();
	void mapVboDataToVertexAttributeLocation();
	void uploadVertexDataToVbos();

	void initLineData();

	void setLineColour(const glm::vec3 & colour);

	void reset();
	void initProjectionMatrices();
	void initModelMatrices();
	void initViewMatrices();
	void initCube();
	void initGnomon();
	void initViewPort();

	glm::vec2 mapWindowToViewPort(const glm::vec2 & point);
	glm::vec3 mapWindowToViewPort(const glm::vec3 & point);
	Line mapWindowToViewPort(const Line & line);
	Line clipToPlane(const glm::vec3 & point, const glm::vec3 & normal, Line line);
	Line clipToPlane(const glm::vec3 & point, const glm::vec3 & normal, const glm::vec3 & A, const glm::vec3 & B);

	void drawViewPort();
	void drawGnomon(const glm::mat4 & M);
	void drawCube(const glm::mat4 & M);

	glm::vec2 removeZ(glm::vec3);
	glm::vec4 point(glm::vec3 v);

	void drawClippedViewPortLine(const glm::vec3 & v0, const glm::vec3 & v1);
	void drawLine(const Line & line);
	void drawLine (
			const glm::vec2 & v0,
			const glm::vec2 & v1
	); 
	void drawLine (
			const glm::vec3 & v0,
			const glm::vec3 & v1
	);

	void drawLine (
			const glm::vec4 & v0,
			const glm::vec4 & v1
	);

	glm::mat4 makePerspectiveMat4(float angle, float near, float far);
	glm::mat4 makeScaleMat4(float x, float y, float z);
	glm::mat4 makeTranslateMat4(float x, float y, float z);
	glm::mat4 makeRotateXMat4(float theta);
	glm::mat4 makeRotateYMat4(float theta);
	glm::mat4 makeRotateZMat4(float theta);

	glm::mat4 addToScaleMat4(const glm::mat4 & mat, float dx, float dy, float dz);
	glm::mat4 reverseTranslationMat4(const glm::mat4 & mat);
	glm::mat4 makeAxisTranslationMat4(const glm::mat4 & mat, const glm::mat4 & axis);

	void applyTransformationChanges(double xPos, double yPos, double xDiff, double yDiff);

	glm::mat4 multAllMat();

	ShaderProgram m_shader;

	GLuint m_vao;            // Vertex Array Object
	GLuint m_vbo_positions;  // Vertex Buffer Object
	GLuint m_vbo_colours;    // Vertex Buffer Object

	VertexData m_vertexData;

	glm::vec3 m_currentLineColour;

	int currentMode = Mode::ModelRotation;

	double modelScaleFactor = 0.001;
	double modelRotationFactor = 0.5;
	double modelTranslationFactor = 0.01;

	double viewRotationFactor = 0.5;
	double viewTranslationFactor = 0.01;

	double perspectiveFovFactor = 0.5;
	double perspectiveNearPlaneFactor = 0.001;
	double perspectiveFarPlaneFactor = 0.01;

	float minModelScale = 0.001f;

	int windowHeight;
	int windowWidth;

	// view port coordinates
	// X1 is not necessarily greater than X2 or vice versa
	float viewPortX1;
	float viewPortX2;
	float viewPortY1;
	float viewPortY2;

	// Model transformation matrices
	glm::mat4 modelTranslation;
	glm::mat4 modelRotation;
	glm::mat4 modelScale;

	// World transformation matrices
	glm::mat4 worldTranslation;
	glm::mat4 worldRotation;
	glm::mat4 worldScale;

	// View transformation matrices
	glm::mat4 viewTranslation;
	glm::mat4 viewRotation;
	glm::mat4 viewMatrix;

	// Projection matrices
	glm::mat4 projectionMatrix;

	// Projection variables
	float fov;
	float nearPlane;
	float farPlane;

	// Cube coordinates
	glm::vec3 cubeCoords[8];

	// Gnomon coordinates
	glm::vec3 gnomonCoords[4];

	double lastX;
	double lastY;

	// Mouse variables
	int leftMouseDown = 0;
	int middleMouseDown = 0;
	int rightMouseDown = 0;

	double leftMouseXChange;
	double middleMouseXChange;
	double rightMouseXChange;

	double leftMouseYChange;
	double middleMouseYChange;
	double rightMouseYChange;

	double leftMouseXAnchor;
	double leftMouseYAnchor;
};
