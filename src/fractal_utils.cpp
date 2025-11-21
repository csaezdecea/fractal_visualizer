#include "fractal_utils.hpp"

//
// fractal_utils.cpp
// Implementation of shader utilities for fractal rendering.
// These functions provide minimal error handling and terminate on failures,
// making them suitable for small visualization tools.
//


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


std::string readFile(const char* filename) {
    std::ifstream f(filename);
    if (!f.is_open()) { 
        std::cerr << "Cannot open " << filename << "\n"; 
        exit(1);
    }
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}


GLuint compileShader(const char* path, GLenum type) {
    std::string src = readFile(path);
    const char* s = src.c_str();

    GLuint sh = glCreateShader(type);
    glShaderSource(sh, 1, &s, NULL);
    glCompileShader(sh);

    GLint ok;
    glGetShaderiv(sh, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetShaderInfoLog(sh, 2048, NULL, log);
        std::cerr << "Shader compile error (" << path << "):\n" << log << "\n";
        exit(1);
    }

    return sh;
}

// -----------------------------------------------------
// createProgram
// Creates a traditional graphics pipeline program (VS + FS).
// On link error, prints the log and exits.
// -----------------------------------------------------
GLuint createProgram(const char* vsPath, const char* fsPath) {
    GLuint vs = compileShader(vsPath, GL_VERTEX_SHADER);
    GLuint fs = compileShader(fsPath, GL_FRAGMENT_SHADER);

    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);

    GLint ok;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetProgramInfoLog(p, 2048, NULL, log);
        std::cerr << "Link error:\n" << log << "\n";
        exit(1);
    }

    // Shaders no longer needed after linking.
    glDeleteShader(vs);
    glDeleteShader(fs);
    return p;
}

// -----------------------------------------------------
// createCompute
// Creates a compute shader program.
// On link error, prints the log and exits.
// -----------------------------------------------------
GLuint createCompute(const char* csPath) {
    GLuint cs = compileShader(csPath, GL_COMPUTE_SHADER);

    GLuint p = glCreateProgram();
    glAttachShader(p, cs);
    glLinkProgram(p);

    GLint ok;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetProgramInfoLog(p, 2048, NULL, log);
        std::cerr << "Compute link error:\n" << log << "\n";
        exit(1);
    }

    // Shader object can be removed after linking.
    glDeleteShader(cs);
    return p;
}
