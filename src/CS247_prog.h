#ifndef CS247_PROG_H
#define CS247_PROG_H

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <cassert>
#include <vector>
#include <cmath>
#include <utility>
#include <tuple>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

// framework includes
#include "glslprogram.h"
#include "vboquad.h"


////////////////
// Structures //
////////////////

// window size
const unsigned int gWindowWidth = 512;
const unsigned int gWindowHeight = 512;

int current_scalar_field;
int data_size;
bool en_arrow;
bool en_streamline;
bool en_pathline;

int sampling_rate;
float dt;




//////////////////////
//  Global defines  //
//////////////////////
#define TIMER_FREQUENCY_MILLIS  50

//////////////////////
// Global variables //
//////////////////////

// Handle of the window we're rendering to
static GLFWwindow* window;

char bmModifiers;	// keyboard modifiers (e.g. ctrl,...)

int clearColor;

// data handling
char* filenames[ 3 ];
bool grid_data_loaded;
bool scalar_data_loaded;
unsigned short vol_dim[ 3 ];
float* vector_array;
float* scalar_fields;
float* scalar_bounds;

GLuint scalar_field_texture;

int num_scalar_fields;
int num_timesteps; //stores number of time steps

int loaded_file;
int loaded_timestep;
float timestep;

int view_width, view_height; // height and width of entire view

GLuint displayList_idx;

int toggle_xy;

////////////////
// Prototypes //
////////////////

void drawGlyphs();

void computeStreamline(int x, int y);

void computePathline(int x, int y, int t);

void loadNextTimestep( void );

void LoadData( char* base_filename );
void LoadVectorData( const char* filename );

void DownloadScalarFieldAsTexture( void );
void initGL( void );

void reset_rendering_props( void );

// TODO: define data arrays, VAO and VBO
// Hint: you need one for the glyphs, streamlines, pathlines

// Streamline / Pathline seeds & data storage
struct StreamlineData { std::vector<glm::vec2> points; int method; };
struct PathlineData { std::vector<glm::vec2> points; int method; };
struct SLSeed { int x, y, method; };
struct PLSeed { int x, y, t, method; };
std::vector<SLSeed> streamline_seeds;
std::vector<PLSeed> pathline_seeds;
std::vector<StreamlineData> streamlines;
std::vector<PathlineData> pathlines;

// VAO/VBO handles for overlays
GLuint glyph_VAO, glyph_VBO;
GLuint streamline_VAO, streamline_VBO;
GLuint pathline_VAO, pathline_VBO;

// TODO: define colormap variables
// Hint: you need a colormap mode (off/rainbow/cool-warm) and a blend factor
int colormap_mode;
float blend_factor;
int integration_method;    // 0=Euler, 1=RK2, 2=RK4
int arrow_length_mode;     // 0=constant, 1=speed-proportional
int rake_mode;             // 0=off, 1=horizontal, 2=vertical

// make quad to load texture to
VBOQuad quad;

// GLSL
GLSLProgram vectorProgram;
glm::mat4 model;


#endif //CS247_PROG_H
