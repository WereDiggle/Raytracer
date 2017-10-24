#include "A3.hpp"
#include "scene_lua.hpp"
using namespace std;

#include "cs488-framework/GlErrorCheck.hpp"
#include "cs488-framework/MathUtils.hpp"
#include "GeometryNode.hpp"
#include "JointNode.hpp"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <sys/time.h>

using namespace glm;

static bool show_gui = true;

const size_t CIRCLE_PTS = 48;

//----------------------------------------------------------------------------------------
// Constructor
A3::A3(const std::string & luaSceneFile)
	: m_luaSceneFile(luaSceneFile),
	  m_positionAttribLocation(0),
	  m_normalAttribLocation(0),
	  m_vao_meshData(0),
	  m_vbo_vertexPositions(0),
	  m_vbo_vertexNormals(0),
	  m_vao_arcCircle(0),
	  m_vbo_arcCircle(0)
{

}

//----------------------------------------------------------------------------------------
// Destructor
A3::~A3()
{

}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A3::init()
{
	// Set the background colour.
	glClearColor(0.35, 0.35, 0.35, 1.0);

	createShaderProgram();

	glGenVertexArrays(1, &m_vao_arcCircle);
	glGenVertexArrays(1, &m_vao_meshData);
	enableVertexShaderInputSlots();

	processLuaSceneFile(m_luaSceneFile);

	// Load and decode all .obj files at once here.  You may add additional .obj files to
	// this list in order to support rendering additional mesh types.  All vertex
	// positions, and normals will be extracted and stored within the MeshConsolidator
	// class.
	unique_ptr<MeshConsolidator> meshConsolidator (new MeshConsolidator{
			getAssetFilePath("cylinder.obj"),
			getAssetFilePath("cube.obj"),
			getAssetFilePath("sphere.obj"),
			getAssetFilePath("suzanne.obj")
	});


	// Acquire the BatchInfoMap from the MeshConsolidator.
	meshConsolidator->getBatchInfoMap(m_batchInfoMap);

	// Take all vertex data within the MeshConsolidator and upload it to VBOs on the GPU.
	uploadVertexDataToVbos(*meshConsolidator);

	mapVboDataToVertexShaderInputLocations();

	initPerspectiveMatrix();

	initViewMatrix();

	initLightSources();

	mapJoints(*m_rootNode);

	// Exiting the current scope calls delete automatically on meshConsolidator freeing
	// all vertex data resources.  This is fine since we already copied this data to
	// VBOs on the GPU.  We have no use for storing vertex data on the CPU side beyond
	// this point.
}

//----------------------------------------------------------------------------------------
void A3::processLuaSceneFile(const std::string & filename) {
	// This version of the code treats the Lua file as an Asset,
	// so that you'd launch the program with just the filename
	// of a puppet in the Assets/ directory.
	// std::string assetFilePath = getAssetFilePath(filename.c_str());
	// m_rootNode = std::shared_ptr<SceneNode>(import_lua(assetFilePath));

	// This version of the code treats the main program argument
	// as a straightforward pathname.
	m_rootNode = std::shared_ptr<SceneNode>(import_lua(filename));
	if (!m_rootNode) {
		std::cerr << "Could not open " << filename << std::endl;
	}
}

//----------------------------------------------------------------------------------------
void A3::createShaderProgram()
{
	m_shader_picker.generateProgramObject();
	m_shader_picker.attachVertexShader( getAssetFilePath("picker_VertexShader.vs").c_str() );
	m_shader_picker.attachFragmentShader( getAssetFilePath("picker_FragmentShader.fs").c_str() );
	m_shader_picker.link();

	m_shader.generateProgramObject();
	m_shader.attachVertexShader( getAssetFilePath("VertexShader.vs").c_str() );
	m_shader.attachFragmentShader( getAssetFilePath("FragmentShader.fs").c_str() );
	m_shader.link();

	m_shader_arcCircle.generateProgramObject();
	m_shader_arcCircle.attachVertexShader( getAssetFilePath("arc_VertexShader.vs").c_str() );
	m_shader_arcCircle.attachFragmentShader( getAssetFilePath("arc_FragmentShader.fs").c_str() );
	m_shader_arcCircle.link();
}

//----------------------------------------------------------------------------------------
void A3::enableVertexShaderInputSlots()
{

	//-- Enable input slots for m_vao_meshData:
	{
		glBindVertexArray(m_vao_meshData);

		// Enable the colour id shader attrib for "position" when picking
		m_picker_positionAttribLocation = m_shader_picker.getAttribLocation("position");
		glEnableVertexAttribArray(m_picker_positionAttribLocation);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_positionAttribLocation = m_shader.getAttribLocation("position");
		glEnableVertexAttribArray(m_positionAttribLocation);

		// Enable the vertex shader attribute location for "normal" when rendering.
		m_normalAttribLocation = m_shader.getAttribLocation("normal");
		glEnableVertexAttribArray(m_normalAttribLocation);

		CHECK_GL_ERRORS;
	}

	//-- Enable input slots for m_vao_arcCircle:
	{
		glBindVertexArray(m_vao_arcCircle);

		// Enable the vertex shader attribute location for "position" when rendering.
		m_arc_positionAttribLocation = m_shader_arcCircle.getAttribLocation("position");
		glEnableVertexAttribArray(m_arc_positionAttribLocation);

		CHECK_GL_ERRORS;
	}

	// Restore defaults
	glBindVertexArray(0);
}

//----------------------------------------------------------------------------------------
void A3::uploadVertexDataToVbos (
		const MeshConsolidator & meshConsolidator
) {
	// Generate VBO to store all vertex position data
	{
		glGenBuffers(1, &m_vbo_vertexPositions);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexPositionBytes(),
				meshConsolidator.getVertexPositionDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store all vertex normal data
	{
		glGenBuffers(1, &m_vbo_vertexNormals);

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);

		glBufferData(GL_ARRAY_BUFFER, meshConsolidator.getNumVertexNormalBytes(),
				meshConsolidator.getVertexNormalDataPtr(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}

	// Generate VBO to store the trackball circle.
	{
		glGenBuffers( 1, &m_vbo_arcCircle );
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo_arcCircle );

		float *pts = new float[ 2 * CIRCLE_PTS ];
		for( size_t idx = 0; idx < CIRCLE_PTS; ++idx ) {
			float ang = 2.0 * M_PI * float(idx) / CIRCLE_PTS;
			pts[2*idx] = cos( ang );
			pts[2*idx+1] = sin( ang );
		}

		glBufferData(GL_ARRAY_BUFFER, 2*CIRCLE_PTS*sizeof(float), pts, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		CHECK_GL_ERRORS;
	}
}

//----------------------------------------------------------------------------------------
void A3::mapVboDataToVertexShaderInputLocations()
{
	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_meshData);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
	glVertexAttribPointer(m_picker_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexPositions" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexPositions);
	glVertexAttribPointer(m_positionAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Tell GL how to map data from the vertex buffer "m_vbo_vertexNormals" into the
	// "normal" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertexNormals);
	glVertexAttribPointer(m_normalAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;

	// Bind VAO in order to record the data mapping.
	glBindVertexArray(m_vao_arcCircle);

	// Tell GL how to map data from the vertex buffer "m_vbo_arcCircle" into the
	// "position" vertex attribute location for any bound vertex shader program.
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_arcCircle);
	glVertexAttribPointer(m_arc_positionAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//-- Unbind target, and restore default values:
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
void A3::initPerspectiveMatrix()
{
	float aspect = ((float)m_windowWidth) / m_windowHeight;
	m_perpsective = glm::perspective(degreesToRadians(60.0f), aspect, 0.1f, 100.0f);
}


//----------------------------------------------------------------------------------------
void A3::initViewMatrix() {
	m_view = glm::lookAt(vec3(testDistance * glm::sin(testRotation), 0.0f,testDistance * glm::cos(testRotation)), vec3(0.0f, 0.0f, 0.0f),
			vec3(0.0f, 1.0f, 0.0f));
}

//----------------------------------------------------------------------------------------
void A3::initLightSources() {
	// World-space position
	m_light.position = vec3(-2.0f, 5.0f, 0.5f);
	m_light.rgbIntensity = vec3(0.8f); // White light
}

void A3::mapJoints(SceneNode & node) {

	// map the node id to the node if it's a joint
	if (node.m_nodeType == NodeType::JointNode) {
		JointNode & jointNode = static_cast<JointNode &>(node);
		m_jointMap[node.m_nodeId] = &jointNode;
		cout << "joint ID: " << node.m_nodeId << endl;
	}

	// Recurse
	for (SceneNode * childNode : node.children) {
		mapJoints(*childNode);
	}
}

//----------------------------------------------------------------------------------------
void A3::uploadCommonSceneUniforms() {
	m_shader.enable();
	{
		//-- Set Perpsective matrix uniform for the scene:
		GLint location = m_shader.getUniformLocation("Perspective");
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));
		CHECK_GL_ERRORS;

		//-- Set LightSource uniform for the scene:
		{
			location = m_shader.getUniformLocation("light.position");
			glUniform3fv(location, 1, value_ptr(m_light.position));
			location = m_shader.getUniformLocation("light.rgbIntensity");
			glUniform3fv(location, 1, value_ptr(m_light.rgbIntensity));
			CHECK_GL_ERRORS;
		}

		//-- Set background light ambient intensity
		{
			location = m_shader.getUniformLocation("ambientIntensity");
			vec3 ambientIntensity(0.05f);
			glUniform3fv(location, 1, value_ptr(ambientIntensity));
			CHECK_GL_ERRORS;
		}
	}
	m_shader.disable();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A3::appLogic()
{
	// Place per frame, application logic here ...

	uploadCommonSceneUniforms();
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A3::guiLogic()
{
	if( !show_gui ) {
		return;
	}

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
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		ImGui::RadioButton("Position/Orientation (P)", &curMouseMode, MouseMode::Model);
		ImGui::RadioButton("Joints (J)", &curMouseMode, MouseMode::Joint);

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();
}

//----------------------------------------------------------------------------------------
// Update mesh specific shader uniforms:
static void updateShaderUniforms(
		const ShaderProgram & shader,
		const GeometryNode & node,
		const glm::mat4 & viewMatrix,
		const glm::mat4 & parentTransMatrix
) {

	shader.enable();
	{
		//-- Set ModelView matrix:
		GLint location = shader.getUniformLocation("ModelView");
		mat4 modelView = viewMatrix * parentTransMatrix * node.trans;
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));
		CHECK_GL_ERRORS;

		//-- Set NormMatrix:
		location = shader.getUniformLocation("NormalMatrix");
		mat3 normalMatrix = glm::transpose(glm::inverse(mat3(modelView)));
		glUniformMatrix3fv(location, 1, GL_FALSE, value_ptr(normalMatrix));
		CHECK_GL_ERRORS;


		//-- Set Material values:
		location = shader.getUniformLocation("material.kd");
		vec3 kd = node.material.kd;
		// TODO: just testing. Use colour shifting over time
		if (node.isSelected) {
			struct timeval timer;
			gettimeofday(&timer, NULL);
			float milliseconds = timer.tv_usec / 1000000.0f;
			time_t seconds = timer.tv_sec;
			milliseconds = milliseconds + (seconds % 3);
			milliseconds = milliseconds * (360.0f / 3.0f);
			milliseconds = glm::radians(milliseconds);
			kd = vec3(glm::cos(milliseconds),glm::cos(milliseconds + M_PI_4),glm::sin(milliseconds));
		}
		glUniform3fv(location, 1, value_ptr(kd));
		CHECK_GL_ERRORS;
		location = shader.getUniformLocation("material.ks");
		vec3 ks = node.material.ks;
		glUniform3fv(location, 1, value_ptr(ks));
		CHECK_GL_ERRORS;
		location = shader.getUniformLocation("material.shininess");
		glUniform1f(location, node.material.shininess);
		CHECK_GL_ERRORS;

	}
	shader.disable();

}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A3::draw() {

	glEnable( GL_DEPTH_TEST );
	renderSceneGraph(*m_rootNode);


	glDisable( GL_DEPTH_TEST );
	renderArcCircle();
}

//----------------------------------------------------------------------------------------
void A3::renderSceneGraph(const SceneNode & root) {

	// Bind the VAO once here, and reuse for all GeometryNode rendering below.
	glBindVertexArray(m_vao_meshData);

	// This is emphatically *not* how you should be drawing the scene graph in
	// your final implementation.  This is a non-hierarchical demonstration
	// in which we assume that there is a list of GeometryNodes living directly
	// underneath the root node, and that we can draw them in a loop.  It's
	// just enough to demonstrate how to get geometry and materials out of
	// a GeometryNode and onto the screen.

	// You'll want to turn this into recursive code that walks over the tree.
	// You can do that by putting a method in SceneNode, overridden in its
	// subclasses, that renders the subtree rooted at every node.  Or you
	// could put a set of mutually recursive functions in this class, which
	// walk down the tree from nodes of different types.
	renderNode(root, glm::mat4());

	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

// Renders the given node and all children nodes
void A3::renderNode(const SceneNode & node, const glm::mat4 & parentTransMatrix) {

	// Render the current node if it's a geometry node
	glm::mat4 curTransMatrix = parentTransMatrix * node.get_transform();
	if (node.m_nodeType == NodeType::GeometryNode) {
		const GeometryNode & geometryNode = static_cast<const GeometryNode &>(node);

		updateShaderUniforms(m_shader, geometryNode, m_view, parentTransMatrix);

		// Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
		BatchInfo batchInfo = m_batchInfoMap[geometryNode.meshId];

		//-- Now render the mesh:
		m_shader.enable();
		glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
		m_shader.disable();
	}
	else if (node.m_nodeType == NodeType::JointNode) {
		const JointNode & jointNode = static_cast<const JointNode &>(node);
		curTransMatrix = parentTransMatrix * jointNode.getJointTransform();
	}

	// Render it's children
	for (const SceneNode * childNode : node.children) {
		renderNode(*childNode, curTransMatrix);
	}
}

glm::vec4 A3::intToColour(unsigned int i) {
	int r = (i & 0x000000FF) >> 0;
	int g = (i & 0x0000FF00) >> 8;
	int b = (i & 0x00FF0000) >> 16;

	return glm::vec4(r/255.0f,
						g/255.0f,
						b/255.0f,
						1.0f);
}

void A3::renderPickingNode(const SceneNode & node, const glm::mat4 & parentTransMatrix, unsigned int jointId) {

	// Render the current node if it's a geometry node
	glm::mat4 curTransMatrix = parentTransMatrix * node.get_transform(); 

	// use 0 for not-a-joint, since only the root will have 0
	unsigned int curJointId = jointId;
	if (node.m_nodeType == NodeType::GeometryNode) {
		const GeometryNode & geometryNode = static_cast<const GeometryNode &>(node);

		m_shader_picker.enable();

		GLint location = m_shader_picker.getUniformLocation("ModelView");
		mat4 modelView = m_view * parentTransMatrix * node.trans;
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(modelView));

		location = m_shader_picker.getUniformLocation("Perspective");
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(m_perpsective));

		location = m_shader_picker.getUniformLocation("PickingColor");
		// We only want the geometry nodes that are children of joint nodes to be selectable.
		// It does not need to be a direct child though.
		glUniform4fv(location, 1, value_ptr(intToColour(curJointId)));

		CHECK_GL_ERRORS;

		// Get the BatchInfo corresponding to the GeometryNode's unique MeshId.
		BatchInfo batchInfo = m_batchInfoMap[geometryNode.meshId];

		//-- Now render the mesh:
		glDrawArrays(GL_TRIANGLES, batchInfo.startIndex, batchInfo.numIndices);
		m_shader_picker.disable();
	}
	else if (node.m_nodeType == NodeType::JointNode) {
		curJointId = node.m_nodeId;
		const JointNode & jointNode = static_cast<const JointNode &>(node);
		curTransMatrix = parentTransMatrix * jointNode.getJointTransform();
	}

	// Render it's children
	for (const SceneNode * childNode : node.children) {
		renderPickingNode(*childNode, curTransMatrix, curJointId);
	}
}

// Highlights the joint and all children nodes until next joint is reached
void A3::highlightJoint(SceneNode & node, bool highlight) {

	if (node.m_nodeType == NodeType::JointNode) {
		node.isSelected = highlight;
		// highlight it's children
		for (SceneNode * childNode : node.children) {
			highlightNodes(*childNode, highlight);
		}
	}
}

void A3::highlightNodes(SceneNode & node, bool highlight) {
	if (node.m_nodeType == NodeType::GeometryNode) {
		node.isSelected = highlight;
		for (SceneNode * childNode : node.children) {
			highlightNodes(*childNode, highlight);
		}
	}
}

unsigned int A3::pickJointUnderMouse() {

	glEnable( GL_DEPTH_TEST );

	// clear buffers
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Bind all the vertex info
	glBindVertexArray(m_vao_meshData);

	renderPickingNode(*m_rootNode, glm::mat4());

	glFlush();
	glFinish();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char data[4];
	double mouseX, mouseY;
	int windowWidth, windowHeight;
	glfwGetCursorPos(m_window, &mouseX, &mouseY);
	glfwGetWindowSize(m_window, &windowWidth, &windowHeight);
	cout << mouseX << " " << windowHeight - mouseY << endl;
	glReadPixels(mouseX, windowHeight - mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

	unsigned int pickedId = data[0] + data[1] * 256 + data[2] * 256 *256;
	cout << pickedId << endl;

	glClearColor(0.35, 0.35, 0.35, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(0);

	glDisable( GL_DEPTH_TEST );

	CHECK_GL_ERRORS;

	if (m_jointMap.find(pickedId) != m_jointMap.end()) {
		SceneNode* selectedJoint = m_jointMap[pickedId];
		highlightJoint(*selectedJoint, !selectedJoint->isSelected);
		cout << "valid joint selected" << endl;
	}

	return pickedId;
}

//----------------------------------------------------------------------------------------
// Draw the trackball circle.
void A3::renderArcCircle() {
	glBindVertexArray(m_vao_arcCircle);

	m_shader_arcCircle.enable();
		GLint m_location = m_shader_arcCircle.getUniformLocation( "M" );
		float aspect = float(m_framebufferWidth)/float(m_framebufferHeight);
		glm::mat4 M;
		if( aspect > 1.0 ) {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5/aspect, 0.5, 1.0 ) );
		} else {
			M = glm::scale( glm::mat4(), glm::vec3( 0.5, 0.5*aspect, 1.0 ) );
		}
		glUniformMatrix4fv( m_location, 1, GL_FALSE, value_ptr( M ) );
		glDrawArrays( GL_LINE_LOOP, 0, CIRCLE_PTS );
	m_shader_arcCircle.disable();

	glBindVertexArray(0);
	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A3::cleanup()
{

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A3::cursorEnterWindowEvent (
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
bool A3::mouseMoveEvent (
		double xPos,
		double yPos
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	switch(curMouseMode) {
		case MouseMode::Model:
			if (leftMouseDown) {
				// TODO: mouse model left right up down relative to screen
			}
			if (middleMouseDown) {
				// TODO: mouse model should move along z axis with mouse Y
			}
			if (rightMouseDown) {
				// TODO: 3D trackball for rotation of model
			}
			break;
		case MouseMode::Joint:
			if (middleMouseDown || rightMouseDown) {
				// TODO: change angles of all selected joints along X axis
				// TODO: change angles of all selected joints along Y axis
				rotateAllSelectedJoints((xPos-lastMouseX) * middleMouseDown, (yPos-lastMouseY) * rightMouseDown);

			}
			break;
	}

	lastMouseX = xPos;
	lastMouseY = yPos;

	return eventHandled;
}

void A3::rotateAllSelectedJoints(double dx, double dy) {
	for (map<unsigned int, JointNode* >::iterator it = m_jointMap.begin(); it != m_jointMap.end(); ++it) {
		cout << "before it->second->isSelected" << endl;
		cout << it->first << endl;
		if (it->second->isSelected) {
			// TODO: change rotation of joint by x and y, keeping it limited to max and min
			cout << "before it->second->rotate X" << endl;
			it->second->rotateJoint('x', (float)dx);
			cout << "before it->second->rotate Y" << endl;
			it->second->rotateJoint('y', (float)dy);
		}
		cout << "after it->second->isSelected" << endl;
	}

}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A3::mouseButtonInputEvent (
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
			pickJointUnderMouse();
			leftMouseDown = 0;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (actions == GLFW_PRESS && !ImGui::IsMouseHoveringAnyWindow()) {
			rightMouseDown = 1;
		}
		else if (actions == GLFW_RELEASE) {
			rightMouseDown = 0;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (actions == GLFW_PRESS && !ImGui::IsMouseHoveringAnyWindow()) {
			middleMouseDown = 1;
		}
		else if (actions == GLFW_RELEASE) {
			middleMouseDown = 0;
		}
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A3::mouseScrollEvent (
		double xOffSet,
		double yOffSet
) {
	bool eventHandled(false);

	// Fill in with event handling code...
	testRotation+= (float) yOffSet * 0.1;
	m_view = glm::lookAt(vec3(testDistance * glm::sin(testRotation), 0.0f,testDistance * glm::cos(testRotation)), vec3(0.0f, 0.0f, 0.0f),
			vec3(0.0f, 1.0f, 0.0f));

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A3::windowResizeEvent (
		int width,
		int height
) {
	bool eventHandled(false);
	initPerspectiveMatrix();
	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A3::keyInputEvent (
		int key,
		int action,
		int mods
) {
	bool eventHandled(false);

	if( action == GLFW_PRESS ) {
		if( key == GLFW_KEY_M ) {
			show_gui = !show_gui;
			eventHandled = true;
		}
		else if ( key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		else if ( key == GLFW_KEY_I) {
			// TODO: reset position
		}
		else if ( key == GLFW_KEY_O) {
			// TODO: reset orientation
		}
		else if ( key == GLFW_KEY_N) {
			// TODO: reset all joint angles. Clears undo/redo stack
		}
		else if ( key == GLFW_KEY_A) {
			// TODO: reset all the things
		}
		else if ( key == GLFW_KEY_U) {
			// TODO: undo the last change
		}
		else if ( key == GLFW_KEY_R) {
			// TODO: redo the change
		}
		else if ( key == GLFW_KEY_C) {
			// TODO: draw circle for trackball
		}
		else if ( key == GLFW_KEY_Z) {
			// TODO: use depth buffering
		}
		else if ( key == GLFW_KEY_Z) {
			// TODO: use depth buffering
		}
		else if ( key == GLFW_KEY_B) {
			// TODO: backface culling
		}
		else if ( key == GLFW_KEY_F) {
			// TODO: frontface culling
		}
		else if ( key == GLFW_KEY_P) {
			curMouseMode = MouseMode::Model;
		}
		else if ( key == GLFW_KEY_J) {
			curMouseMode = MouseMode::Joint;
		}
	}
	// Fill in with event handling code...

	return eventHandled;
}
