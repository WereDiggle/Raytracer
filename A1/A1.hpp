#pragma once

#include <glm/glm.hpp>

#include "cs488-framework/CS488Window.hpp"
#include "cs488-framework/OpenGLImport.hpp"
#include "cs488-framework/ShaderProgram.hpp"

#include "grid.hpp"

struct cell
{
	int height;
	int colour;	

	cell() : 
	height(0), colour(0)
	{

	}

};

class A1 : public CS488Window {
public:
	A1();
	virtual ~A1();

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

private:
	void initGrid();
	void initCube();
	void initWireCube();

	void resetColours();
	void resetCells();
	void reset();

	// Fields related to the shader and uniforms.
	ShaderProgram m_shader;
	GLint P_uni; // Uniform location for Projection matrix.
	GLint V_uni; // Uniform location for View matrix.
	GLint M_uni; // Uniform location for Model matrix.
	GLint col_uni;   // Uniform location for cube colour.

	// Fields related to grid geometry.
	GLuint m_grid_vao; // Vertex Array Object
	GLuint m_grid_vbo; // Vertex Buffer Object

	// Fields related to cube geometry.
	GLuint m_cube_vao; // Vertex Array Object
	GLuint m_cube_vbo; // Vertex Buffer Object

	// Fields related to wire cube geometry.
	GLuint m_wire_cube_vao; // Vertex Array Object
	GLuint m_wire_cube_vbo; // Vertex Buffer Object

	// Active cell
	int activeX;
	int activeY;

	// Cells
	cell** cells;

	// for handling holding shift
	int shiftDown = 0;

	// mouse input
	int dragging = 0;
	float lastX = 0;
	float rotateX = 0;
	float rotateFactor = 0.005;
	
	float scale = 1;
	float scaleFactor = 0.1;

	// Matrices controlling the camera and projection
	glm::mat4 proj;
	glm::mat4 view;

	float colours[8][3];
	int current_col;
};
