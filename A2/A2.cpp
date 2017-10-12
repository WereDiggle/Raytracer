#include "A2.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
using namespace std;

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/ext.hpp>
using namespace glm;

//----------------------------------------------------------------------------------------
// Constructor
VertexData::VertexData()
	: numVertices(0),
	  index(0)
{
	positions.resize(kMaxVertices);
	colours.resize(kMaxVertices);
}


//----------------------------------------------------------------------------------------
// Constructor
A2::A2()
	: m_currentLineColour(vec3(0.0f))
{

}

//----------------------------------------------------------------------------------------
// Destructor
A2::~A2()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A2::init()
{
	// Set the background colour.
	glClearColor(0.3, 0.5, 0.7, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao);

	enableVertexAttribIndices();

	generateVertexBuffers();

	mapVboDataToVertexAttributeLocation();

	initGnomon();
	initCube();
	initModelMatrices();
	initViewMatrices();
	initProjectionMatrices();
	initViewPort();
}

//----------------------------------------------------------------------------------------
/*
 * resets all transformations;
 */
void A2::reset() {
	initModelMatrices();
	initViewMatrices();
	initProjectionMatrices();
	initViewPort();
	currentMode = Mode::ModelRotation;
}

//----------------------------------------------------------------------------------------
/*
 * Multiplies all matrices together and returns the result
 */
glm::mat4 A2::multAllMat()
{
	// Need to translate back to the origin first
	return 	//reverseTranslationMat4(modelTranslation) * 
			modelTranslation *
			modelRotation * 
			modelScale; 
}

//---------------------------------------------------------------------------------------
/*
 * given an fov angle, a far plane, and a near plane. Returns a perspective projection matrix
 */
glm::mat4 A2::makePerspectiveMat4(float angle, float near, float far)
{
	float angleRad = glm::radians(angle);
	float m[16] = {
		1.0f / tan(angleRad / 2.0f), 0, 0, 0,
		0, 1.0f / tan(angleRad / 2.0f), 0, 0,
		0, 0, -1 * ((far + near)/(far - near)), -1,
		0, 0, (-2 * far * near)/(far - near),  0
	};

	return glm::make_mat4(m);
}

//---------------------------------------------------------------------------------------
/*
 * Given a translation matrix and a rotation matrix, translate along the rotational axis.
 */
glm::mat4 A2::makeAxisTranslationMat4(const glm::mat4 & mat, const glm::mat4 & axis)
{
	glm::mat4 retMat = axis * mat;

	// Set the non-translation parts of the matrix to be standard translation
	for (int i=0; i<3; i++) {
		for (int j=0; j<4; j++) {
			if (i == j) {
				retMat[i][j] = 1;
			}
			else {
				retMat[i][j] = 0;
			}
		}
	}

	return retMat;
}

//----------------------------------------------------------------------------------------
/*
 * Given a translation matrix, reverses it.
 */
glm::mat4 A2::reverseTranslationMat4(const glm::mat4 & mat)
{
	glm::mat4 retMat = glm::mat4(mat);

	// negate each translation
	for (int i=0; i<3; i++) {
		retMat[3][i] = -1 * retMat[3][i];
	}

	printf("retMat: %s\n", glm::to_string(retMat).c_str());
	printf("mat: %s\n", glm::to_string(mat).c_str());

	return retMat;
}

//----------------------------------------------------------------------------------------
/*
 * Given a scale matrix, adds to the scale factor. Won't add past a minimum
 */
glm::mat4 A2::addToScaleMat4(const glm::mat4 & mat, float dx, float dy, float dz)
{
	float m[16] = {
		dx, 0, 0, 0,
		0, dy, 0, 0,
		0, 0, dz, 0,
		0, 0, 0,  0
	};
	glm::mat4 retMat = mat + glm::make_mat4(m);

	// Bind each value to a min
	for (int i=0; i<3; i++) {
		retMat[i][i] = glm::max(retMat[i][i], minModelScale);
	}

	return retMat;
}

//----------------------------------------------------------------------------------------
/*
 * Makes a scale transformation matrix
 */
glm::mat4 A2::makeScaleMat4(float x, float y, float z)
{
	float m[16] = {
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	};
	return glm::make_mat4(m);
}

//----------------------------------------------------------------------------------------
/*
 * Makes a translation transformation matrix
 */
glm::mat4 A2::makeTranslateMat4(float x, float y, float z)
{
	// it's actually the transpose of this
	float m[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	};
	return glm::make_mat4(m);
}

//----------------------------------------------------------------------------------------
/*
 * Makes a rotation transformation matrix about axis X. Takes degrees.
 */
glm::mat4 A2::makeRotateXMat4(float theta)
{
	float thetaRad = glm::radians(theta);
	float m[16] = {
		1, 0, 0, 0,
		0, glm::cos(thetaRad), glm::sin(thetaRad), 0,
		0, -1*glm::sin(thetaRad), glm::cos(thetaRad), 0,
		0, 0, 0, 1
	};
	return glm::make_mat4(m);
}

//----------------------------------------------------------------------------------------
/*
 * Makes a rotation transformation matrix about axis Y. Takes degrees.
 */
glm::mat4 A2::makeRotateYMat4(float theta)
{
	float thetaRad = glm::radians(theta);
	float m[16] = {
		glm::cos(thetaRad), 0, -1*glm::sin(thetaRad), 0,
		0, 1, 0, 0,
		glm::sin(thetaRad), 0, glm::cos(thetaRad), 0,
		0, 0, 0, 1
	};
	return glm::make_mat4(m);
}

//----------------------------------------------------------------------------------------
/*
 * Makes a rotation transformation matrix about axis Z. Takes degrees.
 */
glm::mat4 A2::makeRotateZMat4(float theta)
{
	float thetaRad = glm::radians(theta);
	float m[16] = {
		glm::cos(thetaRad), glm::sin(thetaRad), 0, 0,
		-1*glm::sin(thetaRad), glm::cos(thetaRad), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	return glm::make_mat4(m);
}

//----------------------------------------------------------------------------------------
/*
 * Used to initialize view port size
 */
void A2::initViewPort()
{
	glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
	viewPortX1 = -0.95f;
	viewPortY1 = -0.95f;
	viewPortX2 = 0.95f;
	viewPortY2 = 0.95f;
}

//----------------------------------------------------------------------------------------
/*
 * Used to initialize Gnomon coordinates
 */
void A2::initGnomon()
{
	gnomonCoords[0] = glm::vec3(0,0,0);
	gnomonCoords[1] = glm::vec3(0.1f,0,0);
	gnomonCoords[2] = glm::vec3(0,0.1f,0);
	gnomonCoords[3] = glm::vec3(0,0,0.1f);
}

//----------------------------------------------------------------------------------------
/*
 * Used to initialize projection matrices
 */
void A2::initProjectionMatrices() {
	fov = 90;
	nearPlane = -0.1f;
	farPlane = -20.0f;
	projectionMatrix = makePerspectiveMat4(fov, nearPlane, farPlane);
}

//----------------------------------------------------------------------------------------
/*
 * Used to initialize view matrices
 */
void A2::initViewMatrices() {
	viewRotation = makeRotateXMat4(0);
	viewTranslation = makeTranslateMat4(0,0,-1);
	viewMatrix = viewTranslation * viewRotation;
}

//----------------------------------------------------------------------------------------
/*
 * Used to initialize model matrices
 */
void A2::initModelMatrices() {
	modelScale = makeScaleMat4(0.1,0.1,0.1);
	modelRotation = makeRotateXMat4(0);
	modelTranslation = makeTranslateMat4(0,0,0);
}

//----------------------------------------------------------------------------------------
/*
 * Used to initialize Cube coordinates. and model matrices
 */
void A2::initCube()
{
	cubeCoords[0] = glm::vec3(-1,-1,-1);
	cubeCoords[1] = glm::vec3(-1,-1,1);
	cubeCoords[2] = glm::vec3(-1,1,-1);
	cubeCoords[3] = glm::vec3(-1,1,1);
	cubeCoords[4] = glm::vec3(1,-1,-1);
	cubeCoords[5] = glm::vec3(1,-1,1);
	cubeCoords[6] = glm::vec3(1,1,-1);
	cubeCoords[7] = glm::vec3(1,1,1);

}

//----------------------------------------------------------------------------------------
/*
 * Converts window coordinates to view port coordinates
 */
glm::vec2 A2::mapWindowToViewPort(const glm::vec2 & point)
{
	return glm::vec2(glm::abs(viewPortX1 - viewPortX2) / 2 * (point.x + 1) + glm::min(viewPortX1, viewPortX2),
					 glm::abs(viewPortY1 - viewPortY2) / 2 * (point.y + 1) + glm::min(viewPortY1, viewPortY2));
}

//----------------------------------------------------------------------------------------
/*
 * Converts window coordinates to view port coordinates
 */
glm::vec3 A2::mapWindowToViewPort(const glm::vec3 & point)
{
	return glm::vec3(glm::abs(viewPortX1 - viewPortX2) / 2 * (point.x + 1) + glm::min(viewPortX1, viewPortX2),
					 glm::abs(viewPortY1 - viewPortY2) / 2 * (point.y + 1) + glm::min(viewPortY1, viewPortY2),
					 point.z);
}

//----------------------------------------------------------------------------------------
/*
 * Converts window Line to view port Line
 */
Line A2::mapWindowToViewPort(const Line & line)
{
	return Line(mapWindowToViewPort(line.A), mapWindowToViewPort(line.B));
}

//----------------------------------------------------------------------------------------
/*
 * clips the given line to plane represented by the point and normal. Returns the clipped line
 */
Line A2::clipToPlane(const glm::vec3 & point, const glm::vec3 & normal, Line line)
{
	Line retLine = Line();
	if (line.valid) {
		retLine = clipToPlane(point, normal, line.A, line.B);
	}
	return retLine;
}

//----------------------------------------------------------------------------------------
/*
 * clips the given line (A,B) to plane represented by the point and normal. Returns the clipped line
 */
Line A2::clipToPlane(const glm::vec3 & point, const glm::vec3 & normal, const glm::vec3 & A, const glm::vec3 & B)
{
	float wecA = glm::dot(A - point, normal);
	float wecB = glm::dot(B - point, normal);

	// completely outside
	if (wecA < 0 && wecB < 0) {
		return Line();
	}

	// completely inside
	if (wecA >= 0 && wecB >= 0) {
		return Line(A, B);
	}

	float t = wecA / (wecA - wecB);
	Line retLine;
	if (wecA < 0) {
		retLine = Line(A + t * (B-A), B);
	}
	else {
		retLine = Line(A, A + t * (B-A));
	}

	return retLine;
}

//----------------------------------------------------------------------------------------
/*
 * Draw a line, projected in perspective and clipped to the bounding volume and mapped to the view port
 */
void A2::drawClippedViewPortLine(const glm::vec3 & v0, const glm::vec3 & v1)
{
	// Front clip
	Line line = clipToPlane(glm::vec3(0,0,nearPlane), glm::vec3(0,0,-1), v0, v1);
	
	// Back clip
	line = clipToPlane(glm::vec3(0,0,farPlane), glm::vec3(0,0,1), line);

	line = Line(vec3((projectionMatrix * point(line.A)) / line.A.z), vec3((projectionMatrix * point(line.B)) / line.B.z));

	//line = clipToPlane(glm::vec3(0,0,1), glm::vec3(0,0,-1), line);

	// Top clip
	line = clipToPlane(glm::vec3(0,1,0), glm::vec3(0,-1,0), line);

	// Bottom clip
	line = clipToPlane(glm::vec3(0,-1,0), glm::vec3(0,1,0), line);

	// Right clip
	line = clipToPlane(glm::vec3(1,0,0), glm::vec3(-1,0,0), line);

	// Left clip
	line = clipToPlane(glm::vec3(-1,0,0), glm::vec3(1,0,0), line);

	if (line.valid) {
		drawLine(mapWindowToViewPort(line));
	}
}

//----------------------------------------------------------------------------------------
/*
 * Draw a Gnomon using the given a transformation matrix
 */
void A2::drawGnomon(const glm::mat4 & M)
{
	glm::vec4 newGnomonCoords[4] = {
		M * point(gnomonCoords[0]),
		M * point(gnomonCoords[1]),
		M * point(gnomonCoords[2]),
		M * point(gnomonCoords[3])
	};

	// red X axis
	setLineColour(vec3(1.0f, 0, 0));
	drawClippedViewPortLine(vec3(newGnomonCoords[0]), vec3(newGnomonCoords[1]));

	// green Y axis
	setLineColour(vec3(0, 1.0f, 0));
	drawClippedViewPortLine(vec3(newGnomonCoords[0]), vec3(newGnomonCoords[2]));

	// blue Z axis
	setLineColour(vec3(0, 0, 1.0f));
	drawClippedViewPortLine(vec3(newGnomonCoords[0]), vec3(newGnomonCoords[3]));
}

//----------------------------------------------------------------------------------------
/*
 * draws the view port
 */
void A2::drawViewPort()
{
	setLineColour(vec3(0.0f, 0.0f, 0.0f));
	drawLine(glm::vec2(viewPortX1, viewPortY1), glm::vec2(viewPortX1, viewPortY2));
	drawLine(glm::vec2(viewPortX1, viewPortY2), glm::vec2(viewPortX2, viewPortY2));
	drawLine(glm::vec2(viewPortX2, viewPortY2), glm::vec2(viewPortX2, viewPortY1));
	drawLine(glm::vec2(viewPortX2, viewPortY1), glm::vec2(viewPortX1, viewPortY1));
}

//----------------------------------------------------------------------------------------
/*
 * Used to draw the cube using the current combined matrix
 */
void A2::drawCube(const glm::mat4 & M)
{
	glm::vec4 newCubeCoords[8] = {
		M * point(cubeCoords[0]),	
		M * point(cubeCoords[1]),	
		M * point(cubeCoords[2]),	
		M * point(cubeCoords[3]),	
		M * point(cubeCoords[4]),	
		M * point(cubeCoords[5]),	
		M * point(cubeCoords[6]),	
		M * point(cubeCoords[7]),	
	};

	//printf("Draw CUBE: \n");
	//for (int i=0; i<8; i++) {
	//	printf("%s\n", glm::to_string(newCubeCoords[i]).c_str());
	//}

	setLineColour(vec3(1.0f, 1.0f, 1.0f));
	drawClippedViewPortLine(vec3(newCubeCoords[1]), vec3(newCubeCoords[3]));
	drawClippedViewPortLine(vec3(newCubeCoords[1]), vec3(newCubeCoords[5]));
	drawClippedViewPortLine(vec3(newCubeCoords[3]), vec3(newCubeCoords[7]));
	drawClippedViewPortLine(vec3(newCubeCoords[5]), vec3(newCubeCoords[7]));

	drawClippedViewPortLine(vec3(newCubeCoords[0]), vec3(newCubeCoords[1]));
	drawClippedViewPortLine(vec3(newCubeCoords[0]), vec3(newCubeCoords[2]));
	drawClippedViewPortLine(vec3(newCubeCoords[0]), vec3(newCubeCoords[4]));
	drawClippedViewPortLine(vec3(newCubeCoords[2]), vec3(newCubeCoords[3]));
	drawClippedViewPortLine(vec3(newCubeCoords[2]), vec3(newCubeCoords[6]));
	drawClippedViewPortLine(vec3(newCubeCoords[4]), vec3(newCubeCoords[5]));
	drawClippedViewPortLine(vec3(newCubeCoords[4]), vec3(newCubeCoords[6]));
	drawClippedViewPortLine(vec3(newCubeCoords[6]), vec3(newCubeCoords[7]));
}

//----------------------------------------------------------------------------------------
/*
 * Turns a vec3 coordinate into a vec4 with a 1 at the end for point representation
 */
glm::vec4 A2::point(glm::vec3 v)
{
	return glm::vec4(v.x, v.y, v.z, 1);
}

//----------------------------------------------------------------------------------------
/*
 * transforms vec3 to vec2 by removing the z coordinate
 */
glm::vec2 A2::removeZ(glm::vec3 v)
{
	return glm::vec2(v.x, v.y);
}

//----------------------------------------------------------------------------------------
/*
 * Wrapper for drawLine. takes a Line struct
 */
void A2::drawLine(const Line & line) 
{
	drawLine(vec2(line.A), vec2(line.B));
}

//----------------------------------------------------------------------------------------
/*
 * Wrapper for drawLine. Takes vec4, but ignores the z and the point 1
 */
void A2::drawLine(
		const glm::vec4 & v0,   // Line Start (NDC coordinate)
		const glm::vec4 & v1    // Line End (NDC coordinate)
) {
	drawLine(removeZ(vec3(v0)), removeZ(vec3(v1)));
}

//----------------------------------------------------------------------------------------
/*
 * Wrapper for drawLine. Takes vec3, but ignores the z
 */
void A2::drawLine(
		const glm::vec3 & v0,   // Line Start (NDC coordinate)
		const glm::vec3 & v1    // Line End (NDC coordinate)
) {
	drawLine(removeZ(v0), removeZ(v1));
}

//----------------------------------------------------------------------------------------
void A2::createShaderProgram()
{
	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();
}

//----------------------------------------------------------------------------------------
void A2::enableVertexAttribIndices()
{
	glBindVertexArray(m_vao);

	// Enable the attribute index location for "position" when rendering.
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray(positionAttribLocation);

	// Enable the attribute index location for "colour" when rendering.
	GLint colourAttribLocation = m_shader.getAttribLocation( "colour" );
	glEnableVertexAttribArray(colourAttribLocation);

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A2::generateVertexBuffers()
{
	// Generate a vertex buffer to store line vertex positions
	{
		glGenBuffers(1, &m_vbo_positions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	// Generate a vertex buffer to store line colors
	{
		glGenBuffers(1, &m_vbo_colours);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);

		// Set to GL_DYNAMIC_DRAW because the data store will be modified frequently.
		glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * kMaxVertices, nullptr,
				GL_DYNAMIC_DRAW);


		// Unbind the target GL_ARRAY_BUFFER, now that we are finished using it.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A2::mapVboDataToVertexAttributeLocation()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao);

	// Tell GL how to map data from the vertex buffer "m_vbo_positions" into the
	// "position" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
	GLint positionAttribLocation = m_shader.getAttribLocation( "position" );
	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_colours" into the
	// "colour" vertex attribute index for any bound shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
	GLint colorAttribLocation = m_shader.getAttribLocation( "colour" );
	glVertexAttribPointer(colorAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//---------------------------------------------------------------------------------------
void A2::initLineData()
{
	m_vertexData.numVertices = 0;
	m_vertexData.index = 0;
}

//---------------------------------------------------------------------------------------
void A2::setLineColour (
		const glm::vec3 & colour
) {
	m_currentLineColour = colour;
}

//---------------------------------------------------------------------------------------
void A2::drawLine(
		const glm::vec2 & v0,   // Line Start (NDC coordinate)
		const glm::vec2 & v1    // Line End (NDC coordinate)
) {

	m_vertexData.positions[m_vertexData.index] = v0;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;
	m_vertexData.positions[m_vertexData.index] = v1;
	m_vertexData.colours[m_vertexData.index] = m_currentLineColour;
	++m_vertexData.index;

	m_vertexData.numVertices += 2;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A2::appLogic()
{
	// Place per frame, application logic here ...

	// Call at the beginning of frame, before drawing lines:
	initLineData();

	// Draw the view port 
	drawViewPort();

	glm::mat4 M = viewMatrix;
	drawGnomon(M);
	
	M = M * modelTranslation * modelRotation;
	drawGnomon(M);

	M = M * modelScale;
	setLineColour(vec3(1.0f, 1.0f, 1.0f));
	drawCube(M);

	//// Draw outer square:
	//setLineColour(vec3(1.0f, 0.7f, 0.8f));
	//drawLine(vec2(-0.5f, 0.5f), vec2(0, 0));
	//drawLine(vec2(0.5f, -0.5f), vec2(0.5f, 0.5f));
	//drawLine(vec2(0.5f, 0.5f), vec2(-0.5f, 0.5f));
	//drawLine(vec2(-0.5f, 0.5f), vec2(-0.5f, -0.5f));


	//// Draw inner square:
	//setLineColour(vec3(0.2f, 1.0f, 1.0f));
	//drawLine(vec2(-0.25f, -0.25f), vec2(0.25f, -0.25f));
	//drawLine(vec2(0.25f, -0.25f), vec2(0.25f, 0.25f));
	//drawLine(vec2(0.25f, 0.25f), vec2(-0.25f, 0.25f));
	//drawLine(vec2(-0.25f, 0.25f), vec2(-0.25f, -0.25f));
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A2::guiLogic()
{
	static bool firstRun(true);
	if (firstRun) {
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		firstRun = false;
	}

	static bool showDebugWindow(true);
	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Properties", &showDebugWindow, ImVec2(100,100), opacity,
			windowFlags);


		// Add more gui elements here here ...


		// Create Button, and check if it was clicked:
		if( ImGui::Button( "Quit Application (Q)" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		if( ImGui::Button( "Reset (A)" ) ) {
			reset();
		}
		ImGui::RadioButton("Model Rotation (R)", &currentMode, Mode::ModelRotation);
		ImGui::RadioButton("Model Scale (S)", &currentMode, Mode::ModelScale);
		ImGui::RadioButton("Model Translation (T)", &currentMode, Mode::ModelTranslation);
		ImGui::RadioButton("View Rotation (O)", &currentMode, Mode::ViewRotation);
		ImGui::RadioButton("View Translation (N)", &currentMode, Mode::ViewTranslation);
		ImGui::RadioButton("View Port (V)", &currentMode, Mode::ViewPort);
		ImGui::RadioButton("Perspective (P)", &currentMode, Mode::Perspective);

		ImGui::Text("Near Plane: %.3f", -nearPlane);
		ImGui::Text("Far Plane: %.3f", -farPlane);
		ImGui::Text("FOV: %.3f degrees", fov);

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
void A2::uploadVertexDataToVbos() {

	//-- Copy vertex position data into VBO, m_vbo_positions:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_positions);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec2) * m_vertexData.numVertices,
				m_vertexData.positions.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}

	//-- Copy vertex colour data into VBO, m_vbo_colours:
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colours);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * m_vertexData.numVertices,
				m_vertexData.colours.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A2::draw()
{
	uploadVertexDataToVbos();

	glBindVertexArray(m_vao);

	m_shader.enable();
		glDrawArrays(GL_LINES, 0, m_vertexData.numVertices);
	m_shader.disable();

	// Restore defaults
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A2::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A2::cursorEnterWindowEvent (
		int entered
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse cursor movement events.
 */
bool A2::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	//modelRotation = makeRotateXMat4((float)xPos);
	//modelRotation = modelRotation * makeRotateYMat4((float)yPos);
	applyTransformationChanges(xPos, yPos, xPos - lastX, yPos - lastY);
	//std::cout << xPos << " " << yPos << endl;

	lastX = xPos;
	lastY = yPos;

	eventHandled = true;

	return eventHandled;
}

void A2::applyTransformationChanges(double xPos, double yPos, double xDiff, double yDiff) {
	switch (currentMode) {
		case Mode::Perspective:
			if (leftMouseDown) {
				fov = glm::clamp(fov + (float)(xDiff * perspectiveFovFactor), 5.0f, 160.0f);	
				projectionMatrix = makePerspectiveMat4(fov, nearPlane, farPlane);
			}
			if (middleMouseDown) {
				nearPlane = glm::clamp(nearPlane - (float)(xDiff * perspectiveNearPlaneFactor), farPlane + 0.001f, -0.001f);
				projectionMatrix = makePerspectiveMat4(fov, nearPlane, farPlane);
			}
			if (rightMouseDown) {
				farPlane = glm::clamp(farPlane - (float)(xDiff * perspectiveFarPlaneFactor), -200.0f, nearPlane - 0.001f);
				projectionMatrix = makePerspectiveMat4(fov, nearPlane, farPlane);
			}
			break;
		case Mode::ViewPort:
			if (leftMouseDown) {
				if (leftMouseDown == 1) {
					viewPortX1 = (float) xPos / (float) windowWidth;
					viewPortX1 = 2 * viewPortX1 - 1;
					viewPortY1 = (float) yPos / (float) windowHeight;
					viewPortY1 = -2 * viewPortY1 + 1;
					leftMouseDown = 2;
				}
				viewPortX2 = (float) xPos / (float) windowWidth;
				viewPortX2 = 2 * viewPortX2 - 1;
				viewPortY2 = (float) yPos / (float) windowHeight;
				viewPortY2 = -2 * viewPortY2 + 1;
			}
			break;
		case Mode::ViewRotation:
			if (leftMouseDown) {
				//viewRotation = viewRotation * makeRotateXMat4((float) (xDiff * viewRotationFactor));	
				viewMatrix = glm::inverse(makeRotateXMat4((float) (xDiff * viewRotationFactor))) * viewMatrix;
			}
			if (middleMouseDown) {
				//viewRotation = viewRotation * makeRotateYMat4((float) (xDiff * viewRotationFactor));	
				viewMatrix = glm::inverse(makeRotateYMat4((float) (xDiff * viewRotationFactor))) * viewMatrix;
			}
			if (rightMouseDown) {
				//viewRotation = viewRotation * makeRotateZMat4((float) (xDiff * viewRotationFactor));	
				viewMatrix = glm::inverse(makeRotateZMat4((float) (xDiff * viewRotationFactor))) * viewMatrix;
			}
			break;
		case Mode::ViewTranslation:
			if (leftMouseDown) {
				//viewTranslation = viewTranslation * makeAxisTranslationMat4(makeTranslateMat4((float) (xDiff * viewTranslationFactor), 0, 0), viewRotation);	
				//viewTranslation = viewTranslation * makeTranslateMat4((float) (xDiff * viewTranslationFactor), 0, 0), viewRotation;
				viewMatrix = glm::inverse(makeTranslateMat4((float) (xDiff * viewTranslationFactor), 0, 0)) * viewMatrix;
			}
			if (middleMouseDown) {
				//viewTranslation = viewTranslation * makeAxisTranslationMat4(makeTranslateMat4(0, (float) (xDiff * viewTranslationFactor), 0), viewRotation);	
				//viewTranslation = viewTranslation * makeTranslateMat4(0, (float) (xDiff * viewTranslationFactor), 0), viewRotation;
				viewMatrix = glm::inverse(makeTranslateMat4(0, (float) (xDiff * viewTranslationFactor), 0)) * viewMatrix;
			}
			if (rightMouseDown) {
				//viewTranslation = viewTranslation * makeAxisTranslationMat4(makeTranslateMat4(0, 0, (float) (xDiff * viewTranslationFactor)), viewRotation);	
				//viewTranslation = viewTranslation * makeTranslateMat4(0, 0, (float) (xDiff * viewTranslationFactor)), viewRotation;
				viewMatrix = glm::inverse(makeTranslateMat4(0, 0, (float) (xDiff * viewTranslationFactor))) * viewMatrix;
			}
			break;
		case Mode::ModelRotation: 
			if (leftMouseDown) {
				modelRotation = modelRotation * makeRotateXMat4((float) (xDiff * modelRotationFactor));	
			}
			if (middleMouseDown) {
				modelRotation = modelRotation * makeRotateYMat4((float) (xDiff * modelRotationFactor));	
			}
			if (rightMouseDown) {
				modelRotation = modelRotation * makeRotateZMat4((float) (xDiff * modelRotationFactor));	
			}
			break;
		case Mode::ModelScale: 
			if (leftMouseDown) {
				modelScale = addToScaleMat4(modelScale, (float) (xDiff * modelScaleFactor), 0, 0);	
				//printf("%s\n", glm::to_string(modelScale).c_str());
			}
			if (middleMouseDown) {
				modelScale = addToScaleMat4(modelScale, 0, (float) (xDiff * modelScaleFactor), 0);	
			}
			if (rightMouseDown) {
				modelScale = addToScaleMat4(modelScale, 0, 0, (float) (xDiff * modelScaleFactor));	
			}
			break;
		case Mode::ModelTranslation: 
			if (leftMouseDown) {
				modelTranslation = modelTranslation * makeAxisTranslationMat4(makeTranslateMat4((float) (xDiff * modelTranslationFactor), 0, 0), modelRotation);	
			}
			if (middleMouseDown) {
				modelTranslation = modelTranslation * makeAxisTranslationMat4(makeTranslateMat4(0, (float) (xDiff * modelTranslationFactor), 0), modelRotation);	
			}
			if (rightMouseDown) {
				modelTranslation = modelTranslation * makeAxisTranslationMat4(makeTranslateMat4(0, 0, (float) (xDiff * modelTranslationFactor)), modelRotation);	
			}
			break;
		default:
			break;
	}
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A2::mouseButtonInputEvent (
		int button,
		int actions,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (actions == GLFW_PRESS && !ImGui::IsMouseHoveringAnyWindow()) {
			leftMouseDown = 1;
		}
		else if (actions == GLFW_RELEASE) {
			leftMouseDown = 0;
			leftMouseXChange = 0;
			leftMouseYChange = 0;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (actions == GLFW_PRESS && !ImGui::IsMouseHoveringAnyWindow()) {
			rightMouseDown = 1;
		}
		else if (actions == GLFW_RELEASE) {
			rightMouseDown = 0;
			rightMouseXChange = 0;
			rightMouseYChange = 0;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (actions == GLFW_PRESS && !ImGui::IsMouseHoveringAnyWindow()) {
			middleMouseDown = 1;
		}
		else if (actions == GLFW_RELEASE) {
			middleMouseDown = 0;
			middleMouseXChange = 0;
			middleMouseYChange = 0;
		}
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A2::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A2::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
	std::cout << windowWidth << " " << windowHeight << endl;

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A2::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		else if (key == GLFW_KEY_R) {
			currentMode = Mode::ModelRotation;
		}
		else if (key == GLFW_KEY_T) {
			currentMode = Mode::ModelTranslation;
		}
		else if (key == GLFW_KEY_S) {
			currentMode = Mode::ModelScale;
		}
		else if (key == GLFW_KEY_O) {
			currentMode = Mode::ViewRotation;
		}
		else if (key == GLFW_KEY_N) {
			currentMode = Mode::ViewTranslation;
		}
		else if (key == GLFW_KEY_V) {
			currentMode = Mode::ViewPort;
		}
		else if (key == GLFW_KEY_P) {
			currentMode = Mode::Perspective;
		}
		else if (key == GLFW_KEY_A) {
			reset();
		}
	}

	return eventHandled;
}
