#include "A1.hpp"
#include "cs488-framework/GlErrorCheck.hpp"

#include <iostream>
#include <ctime>

#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAX_HEIGHT 15
#define MAX_SCALE 3.0f
#define MIN_SCALE 0.2f

using namespace glm;
using namespace std;

static const size_t DIM = 16;
static const GLfloat wireframe_cube[] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f,

	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,

	1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,

	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
};
static const GLfloat g_test_buffer_data[] = {
	// Bottom face
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,

	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	
	// Back face
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	// Left face
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	// Top face
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	
	// Front face
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,

	// Right face
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,

	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
};

//----------------------------------------------------------------------------------------
// Constructor
A1::A1()
	: current_col( 0 )
{
	resetColours();
}

//----------------------------------------------------------------------------------------
// Destructor
A1::~A1()
{}

void A1::resetColours()
{
	// colours r, g, b
	colours[0][0] = 1; // white
	colours[0][1] = 1;
	colours[0][2] = 1;

	colours[1][0] = 0; // black
	colours[1][1] = 0;
	colours[1][2] = 0;

	colours[2][0] = 1; // red
	colours[2][1] = 0;
	colours[2][2] = 0;

	colours[3][0] = 0; // green
	colours[3][1] = 1;
	colours[3][2] = 0;

	colours[4][0] = 0; // blue
	colours[4][1] = 0;
	colours[4][2] = 1;

	colours[5][0] = 1; 
	colours[5][1] = 1;
	colours[5][2] = 0;

	colours[6][0] = 1; 
	colours[6][1] = 0;
	colours[6][2] = 1;

	colours[7][0] = 0; 
	colours[7][1] = 1;
	colours[7][2] = 1;
}

void A1::resetCells()
{
	for (int i=0; i<DIM; i++) {
		for (int j=0; j<DIM; j++) {
			cells[i][j] = cell();
		}
	}
}

void A1::reset()
{
	activeX = 0;
	activeY = 0;

	resetColours();
	current_col = 0;

	resetCells();

	rotateX = 0;
	scale = 1;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, at program start.
 */
void A1::init()
{
	// Set the background colour.
	glClearColor( 0.3, 0.5, 0.7, 1.0 );

	// Build the shader
	m_shader.generateProgramObject();
	m_shader.attachVertexShader(
		getAssetFilePath( "VertexShader.vs" ).c_str() );
	m_shader.attachFragmentShader(
	getAssetFilePath( "FragmentShader.fs" ).c_str() );
	m_shader.link();

	// Set up the uniforms
	P_uni = m_shader.getUniformLocation( "P" );
	V_uni = m_shader.getUniformLocation( "V" );
	M_uni = m_shader.getUniformLocation( "M" );
	col_uni = m_shader.getUniformLocation( "colour" );

	initGrid();
	initCube();
	initWireCube();

	// Set up initial view and projection matrices (need to do this here,
	// since it depends on the GLFW window being set up correctly).
	view = glm::lookAt( 
		glm::vec3( 0.0f, float(DIM)*2.0*M_SQRT1_2, float(DIM)*2.0*M_SQRT1_2 ),
		glm::vec3( 0.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ) );

	proj = glm::perspective( 
		glm::radians( 45.0f ),
		float( m_framebufferWidth ) / float( m_framebufferHeight ),
		1.0f, 1000.0f );
	
	// setup the cells
	cells = new cell*[DIM];
	for (int i=0; i < DIM; i++) {
		cells[i] = new cell[DIM];
	}
	activeX = activeY = 0;
}

void A1::initWireCube()
{
	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_wire_cube_vao );
	glBindVertexArray( m_wire_cube_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_wire_cube_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_wire_cube_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(wireframe_cube),
	wireframe_cube, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	CHECK_GL_ERRORS;
}

void A1::initCube()
{
	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_cube_vao );
	glBindVertexArray( m_cube_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_cube_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_cube_vbo );
	glBufferData( GL_ARRAY_BUFFER, sizeof(g_test_buffer_data),
	g_test_buffer_data, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	CHECK_GL_ERRORS;
}

void A1::initGrid()
{
	size_t sz = 3 * 2 * 2 * (DIM+3);

	float *verts = new float[ sz ];
	size_t ct = 0;
	for( int idx = 0; idx < DIM+3; ++idx ) {
		verts[ ct ] = -1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = idx-1;
		verts[ ct+3 ] = DIM+1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = idx-1;
		ct += 6;

		verts[ ct ] = idx-1;
		verts[ ct+1 ] = 0;
		verts[ ct+2 ] = -1;
		verts[ ct+3 ] = idx-1;
		verts[ ct+4 ] = 0;
		verts[ ct+5 ] = DIM+1;
		ct += 6;
	}

	// Create the vertex array to record buffer assignments.
	glGenVertexArrays( 1, &m_grid_vao );
	glBindVertexArray( m_grid_vao );

	// Create the cube vertex buffer
	glGenBuffers( 1, &m_grid_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, m_grid_vbo );
	glBufferData( GL_ARRAY_BUFFER, sz*sizeof(float),
	verts, GL_STATIC_DRAW );

	// Specify the means of extracting the position values properly.
	GLint posAttrib = m_shader.getAttribLocation( "position" );
	glEnableVertexAttribArray( posAttrib );
	glVertexAttribPointer( posAttrib, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

	// Reset state to prevent rogue code from messing with *my* 
	// stuff!
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// OpenGL has the buffer now, there's no need for us to keep a copy.
	delete [] verts;

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, before guiLogic().
 */
void A1::appLogic()
{
	// Place per frame, application logic here ...
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after appLogic(), but before the draw() method.
 */
void A1::guiLogic()
{
	// We already know there's only going to be one window, so for 
	// simplicity we'll store button states in static local variables.
	// If there was ever a possibility of having multiple instances of
	// A1 running simultaneously, this would break; you'd want to make
	// this into instance fields of A1.
	static bool showTestWindow(false);
	static bool showDebugWindow(true);

	ImGuiWindowFlags windowFlags(ImGuiWindowFlags_AlwaysAutoResize);
	float opacity(0.5f);

	ImGui::Begin("Debug Window", &showDebugWindow, ImVec2(100,100), opacity, windowFlags);
		if( ImGui::Button( "Quit Application" ) ) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}

		if( ImGui::Button( "Reset" ) ) {
			reset();
		}

		// Eventually you'll create multiple colour widgets with
		// radio buttons.  If you use PushID/PopID to give them all
		// unique IDs, then ImGui will be able to keep them separate.
		// This is unnecessary with a single colour selector and
		// radio button, but I'm leaving it in as an example.

		// Prefixing a widget name with "##" keeps it from being
		// displayed.

		// Display eight colours
		for (int i=0; i<8; i++) {
			ImGui::PushID( i );
			if (ImGui::ColorEdit3( "##Colour", colours[i] )) {
				ImGui::SetKeyboardFocusHere(1);
			};
			ImGui::SameLine();
			if( ImGui::RadioButton( "##Col", &current_col, i ) ) {
				// change colour of active cell
				cells[activeX][activeY].colour = current_col;
			}
			ImGui::PopID();
		}
/*
		// For convenience, you can uncomment this to show ImGui's massive
		// demonstration window right in your application.  Very handy for
		// browsing around to get the widget you want.  Then look in 
		// shared/imgui/imgui_demo.cpp to see how it's done.
		if( ImGui::Button( "Test Window" ) ) {
			showTestWindow = !showTestWindow;
		}
*/

		ImGui::Text( "Framerate: %.1f FPS", ImGui::GetIO().Framerate );

	ImGui::End();

	if( showTestWindow ) {
		ImGui::ShowTestWindow( &showTestWindow );
	}
}

//----------------------------------------------------------------------------------------
/*
 * Called once per frame, after guiLogic().
 */
void A1::draw()
{
	// Create a global transformation for the model (centre it).
	mat4 W;
	float worldOffset = -float(DIM)/2.0f;
	W = glm::scale(W, vec3(scale, scale, scale));
	W = glm::rotate(W , rotateX*rotateFactor, vec3(0.0,1.0,0.0));
	W = glm::translate( W, vec3( worldOffset, 0, worldOffset ) );

	m_shader.enable();
		glEnable( GL_DEPTH_TEST );

		glUniformMatrix4fv( P_uni, 1, GL_FALSE, value_ptr( proj ) );
		glUniformMatrix4fv( V_uni, 1, GL_FALSE, value_ptr( view ) );
		glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr( W ) );

		// Just draw the grid
		glBindVertexArray( m_grid_vao );
		// This is for colour, not columns, whatever that would mean
		glUniform3f( col_uni, 1, 1, 1 );
		glDrawArrays( GL_LINES, 0, (3+DIM)*4 );

		// Draw the cubes
		glBindVertexArray( m_cube_vao);
		//glBindVertexArray( m_wire_cube_vao);

		// cube transformations
		mat4 cubeTrans;
		
		// draw all the cubes
		for (int i=0; i<DIM; i++) {
			for (int j=0; j<DIM; j++) {
				int colourIndex = cells[i][j].colour;
				glUniform3f( col_uni, colours[colourIndex][0], colours[colourIndex][1], colours[colourIndex][2] );
				for (int k=0; k<cells[i][j].height; k++) {
					//cubeTrasn = glm::rotate(W , rotateX*rotateFactor, vec3(0.0,1.0,0.0));
					cubeTrans = glm::translate(W, vec3(i, k, j));
					glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(cubeTrans));
					glDrawArrays( GL_TRIANGLES, 0, 12*3);
				}
			}
		}

		// draw wire cube indicator on top of everything else
		glDisable( GL_DEPTH_TEST );
		glBindVertexArray(m_wire_cube_vao);

		// set highliht to be a flashing wire frame. Flash every second
		time_t timer;
		time(&timer);
		glUniform3f(col_uni, timer%3, (timer+1)%3, (timer+2)%3);

		for (int i=0; i <= std::min(cells[activeX][activeY].height, MAX_HEIGHT-1); i++) {
			cubeTrans = glm::translate(W, vec3(activeX, i, activeY));
			glUniformMatrix4fv( M_uni, 1, GL_FALSE, value_ptr(cubeTrans));
			glDrawArrays(GL_LINES, 0, 12*2);
		}

		// Highlight the active square.
	m_shader.disable();

	// Restore defaults
	glBindVertexArray( 0 );

	CHECK_GL_ERRORS;
}

//----------------------------------------------------------------------------------------
/*
 * Called once, after program is signaled to terminate.
 */
void A1::cleanup()
{
	for (int i=0; i < DIM; i++) {
		delete [] cells[i];
	}
	delete [] cells;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles cursor entering the window area events.
 */
bool A1::cursorEnterWindowEvent (
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
bool A1::mouseMoveEvent(double xPos, double yPos) 
{
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// Put some code here to handle rotations.  Probably need to
		// check whether we're *dragging*, not just moving the mouse.
		// Probably need some instance variables to track the current
		// rotation amount, and maybe the previous X position (so 
		// that you can rotate relative to the *change* in X.
		if (dragging) {
			if (dragging == 1) {
				lastX = xPos;
				dragging = 2;
			}
			rotateX += xPos - lastX;
			lastX = xPos;
		}
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse button events.
 */
bool A1::mouseButtonInputEvent(int button, int actions, int mods) {
	bool eventHandled(false);

	if (!ImGui::IsMouseHoveringAnyWindow()) {
		// The user clicked in the window.  If it's the left
		// mouse button, initiate a rotation.
		if (button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_PRESS) {
			dragging = 1;
		}
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_RELEASE) {
		dragging = 0;
	}

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles mouse scroll wheel events.
 */
bool A1::mouseScrollEvent(double xOffSet, double yOffSet) {
	bool eventHandled(false);

	// Zoom in or out.
	scale = scale + yOffSet * scaleFactor;
	scale = std::max(MIN_SCALE, scale);
	scale = std::min(MAX_SCALE, scale);

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles window resize events.
 */
bool A1::windowResizeEvent(int width, int height) {
	bool eventHandled(false);

	// Fill in with event handling code...

	return eventHandled;
}

//----------------------------------------------------------------------------------------
/*
 * Event handler.  Handles key input events.
 */
bool A1::keyInputEvent(int key, int action, int mods) {
	bool eventHandled(false);

	// Fill in with event handling code...
	eventHandled = true;
	if( action == GLFW_PRESS ) {
		// Respond to some key events.
		if (key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(m_window, GL_TRUE);
		}
		else if (key == GLFW_KEY_R) {
			reset();
		}
		else if (key == GLFW_KEY_SPACE) {
			cells[activeX][activeY].height = std::min(MAX_HEIGHT, cells[activeX][activeY].height+1);
			cells[activeX][activeY].colour = current_col;
		}
		else if (key == GLFW_KEY_BACKSPACE) {
			cells[activeX][activeY].height = std::max(0, cells[activeX][activeY].height-1);
			cells[activeX][activeY].colour = current_col;
		}
		else if (key == GLFW_KEY_C) {
			cells[activeX][activeY].colour = current_col;
		}
		else if (key == GLFW_KEY_UP) {
			int prevY = activeY;
			activeY = std::max(activeY-1, 0);
			if (shiftDown) {
				cells[activeX][activeY] = cells[activeX][prevY];	
			}
		}
		else if (key == GLFW_KEY_DOWN) {
			int prevY = activeY;
			activeY = std::min(activeY+1, 15);
			if (shiftDown) {
				cells[activeX][activeY] = cells[activeX][prevY];	
			}
		}
		else if (key == GLFW_KEY_LEFT) {
			int prevX = activeX;
			activeX = std::max(activeX-1, 0);
			if (shiftDown) {
				cells[activeX][activeY] = cells[prevX][activeY];	
			}
		}
		else if (key == GLFW_KEY_RIGHT) {
			int prevX = activeX;
			activeX = std::min(activeX+1, 15);
			if (shiftDown) {
				cells[activeX][activeY] = cells[prevX][activeY];	
			}
		}
		else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
			shiftDown = std::min(shiftDown+1, 2);
		}
		else {
			eventHandled = false;
		}
	}
	else if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
			shiftDown = std::max(shiftDown-1, 0);
		}
		else {
			eventHandled = false;
		}
	}
	else {
		eventHandled = false;
	}

	return eventHandled;
}
