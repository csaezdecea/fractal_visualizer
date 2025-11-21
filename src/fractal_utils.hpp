#ifndef FRACTAL_UTILS_HPP
#define FRACTAL_UTILS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

/*
fractal_utils.hpp
Utility declarations for shader loading/compilation and window resizing.
These are intentionally small helper functions for a fractal visualization
pipeline using OpenGL 4.3 compute shaders.
*/

/*
Resize callback. Called automatically by GLFW when the framebuffer size changes. 
Updates the OpenGL viewport to match the new window size.
*/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/*
Reads the entire contents of a text file and returns it as a std::string.
If the file cannot be opened, prints an error and exits the program.
*/
std::string readFile(const char* filename);

/*
Loads and compiles a shader (vertex/fragment/compute) from a file. 'type' must be GL_VERTEX_SHADER, GL_FRAGMENT_SHADER,
 or GL_COMPUTE_SHADER. On failure, prints the compiler log and terminates the program.
*/
GLuint compileShader(const char* path, GLenum type);

/*
Creates and links an OpenGL program from a vertex+fragment shader pair. Useful for rendering passes.
On failure, prints the linker log and terminates the program.
*/
GLuint createProgram(const char* vsPath, const char* fsPath);

/*
Loads, compiles, and links a compute shader. Used for fractal calculations (writes into an image/texture).
On failure, prints the linker log and exits the program.
*/
GLuint createCompute(const char* csPath);

#endif // FRACTAL_UTILS_HPP