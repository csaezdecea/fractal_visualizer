
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cmath>
 
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
 
#include "Shader.h"
 
// int screen_width{ 1080 };
// int screen_height{ 1080 };
 
int screen_width{ 1600 };
int screen_height{ 1600 };

int num_frames{ 0 };
float last_time{ 0.0f };
 
float vertices[] = 
{
//    x      y      z   
    -1.0f, -1.0f, -0.0f,
     1.0f,  1.0f, -0.0f,
    -1.0f,  1.0f, -0.0f,
     1.0f, -1.0f, -0.0f
};
 
unsigned int indices[] = 
{
//  2---,1
//  | .' |
//  0'---3
    0, 1, 2,
    0, 3, 1
};
 
 
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}
 
void countFPS()
{
    double current_time = glfwGetTime();
    num_frames++;
    if (current_time - last_time >= 1.0)
    {
        std::cout << 1000.0 / num_frames << "ms / frame\n";
        num_frames = 0;
        last_time += 1.0;
    }
}

glm::vec4 on_key(GLFWwindow *window, glm::vec4 coord_op)
{
    glm::vec3 coord(coord_op.x, coord_op.y, coord_op.z);
    float option = coord_op.w;

    if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window,true);

    if (glfwGetKey(window,GLFW_KEY_Z) == GLFW_PRESS)
    coord.z = coord.z*0.99;

    if (glfwGetKey(window,GLFW_KEY_X) == GLFW_PRESS)
    coord.z = coord.z*1.01;

    if (glfwGetKey(window,GLFW_KEY_RIGHT) == GLFW_PRESS)
    coord.x = coord.x-0.002*pow(coord.z,1.0);

    if (glfwGetKey(window,GLFW_KEY_LEFT) == GLFW_PRESS)
    coord.x = coord.x+0.002*pow(coord.z,1.0);

    if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
    coord.y = coord.y-0.002*pow(coord.z,1.0);

    if (glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS)
    coord.y = coord.y+0.002*pow(coord.z,1.0);

    if (glfwGetKey(window,GLFW_KEY_1) == GLFW_PRESS) {
            coord.x = -0.2; coord.y = 0.0; 
            coord.z = 0.8; option = 1.0;
    }

    if (glfwGetKey(window,GLFW_KEY_2) == GLFW_PRESS) {
        coord.x = 0.2; coord.y = 0.0; 
        coord.z = 1.2; option = 2.0;
    }

    if (glfwGetKey(window,GLFW_KEY_3) == GLFW_PRESS) {
        coord.x = 0.0; coord.y = 0.0; 
        coord.z = 0.8; option = 3.0;
    }
    
    if (glfwGetKey(window,GLFW_KEY_4) == GLFW_PRESS) {
        coord.x = 0.0; coord.y = 0.0; 
        coord.z = 1.2; option = 4.0;
    }
    
    glm::vec4 coordop(coord.x,coord.y,coord.z,option);

    return coordop;  
}

 
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    GLFWwindow * window = glfwCreateWindow(screen_width, screen_height, "Conjuntos de Julia", NULL, NULL);
 
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window!\n";
        glfwTerminate();
        return -1;
    }
 
    glfwMakeContextCurrent(window);
 
    if (glewInit())
    {
        std::cout << "Failed initializing GLEW\n";
    }
  
    glViewport(0, 0, screen_width, screen_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
 
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
 
    glBindVertexArray(VAO);
 
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
 
    Shader our_shader("shader.vert", "shader.frag");
 
    last_time = glfwGetTime();
 
    glEnable(GL_DEPTH_TEST);

    glm::vec4 coord_op(-0.2, 0., 0.8, 1.0);

    //float option = 1.0;
 
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
        countFPS();
 
        our_shader.use_shader(coord_op.xyz(), coord_op.w);
        glBindVertexArray(VAO);
         
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 
        glfwSwapBuffers(window);
        glfwPollEvents();

        coord_op = on_key(window,coord_op);

       
    }
 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
 
    glfwTerminate();
    return 0;
}
