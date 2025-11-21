#include "fractal_utils.hpp" 
// Contains: readFile, compileShader, createProgram, createCompute, framebuffer_size_callback

/// -----------------------------------------------------
/// MAIN
/// -----------------------------------------------------
int main() {
    // -----------------------------------------------------
    // Initialize GLFW
    // -----------------------------------------------------
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }

    // Request OpenGL 4.3 Core (needed for compute shaders)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Desired initial window size
    int WIDTH = 900, HEIGHT = 700;

    // Create window + OpenGL context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Double-ish Fractal", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // -----------------------------------------------------
    // Register window resize callback
    // (adjusts glViewport when the framebuffer changes size)
    // -----------------------------------------------------
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Allow key press detection even if key was pressed once
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

    // -----------------------------------------------------
    // Initialize GLEW (loads OpenGL functions)
    // -----------------------------------------------------
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed\n";
        return -1;
    }

    // -----------------------------------------------------
    // Load shaders
    // -----------------------------------------------------
    GLuint computeProgram = createCompute("shaders/shader.comp");          // compute shader
    GLuint renderProgram  = createProgram("shaders/shader.vert",           // fullscreen quad render
                                         "shaders/shader.frag");

    // -----------------------------------------------------
    // Create texture where compute shader will write pixel data
    // -----------------------------------------------------
    GLuint tex; 
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Allocate texture memory (RGBA32F = float per channel)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT,
                 0, GL_RGBA, GL_FLOAT, NULL);

    // Linear filtering (compute shader always writes exact pixels)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // -----------------------------------------------------
    // Fullscreen quad (two triangles as a triangle strip)
    // Vertex format: (x, y, u, v)
    // -----------------------------------------------------
    float quad[] = {
        -1.f,-1.f, 0.f,0.f,
         1.f,-1.f, 1.f,0.f,
        -1.f, 1.f, 0.f,1.f,
         1.f, 1.f, 1.f,1.f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    // Position attribute (first 2 floats)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Tex coordinates (next 2 floats)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    // -----------------------------------------------------
    // Fractal parameters (interactive controls)
    // -----------------------------------------------------
    double centerX = -0.2;
    double centerY = 0.0;
    double zoom = 1.0;
    int option = 1;           // fractal selection
    int maxIterations = 2000;  // computation depth

    // -----------------------------------------------------
    // MAIN LOOP
    // -----------------------------------------------------
    while (!glfwWindowShouldClose(window)) {

        // -------------------------------------------------
        // Handle window resizing safely
        // Reallocates texture and updates viewport
        // -------------------------------------------------
        int fbW, fbH;
        glfwGetFramebufferSize(window, &fbW, &fbH);

        if (fbW != WIDTH || fbH != HEIGHT) {
            WIDTH = fbW;
            HEIGHT = fbH;

            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
                         WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

            glViewport(0, 0, WIDTH, HEIGHT);
        }

        glfwPollEvents(); // process keyboard/mouse events

        // -------------------------------------------------
        // Movement: arrow keys move the fractal center
        // Movement speed scales with zoom
        // -------------------------------------------------
        double moveStep = 0.004 * zoom;

        if (glfwGetKey(window, GLFW_KEY_UP   ) == GLFW_PRESS) centerY -= moveStep;
        if (glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS) centerY += moveStep;
        if (glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS) centerX += moveStep;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) centerX -= moveStep;

        // Zoom controls
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) zoom *= 0.98; // zoom in
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) zoom *= 1.02; // zoom out

        // Switch fractal types (re-centers view)
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) { option = 1; centerX=-0.5; zoom=1.0; }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) { option = 2; centerX= 0.0; zoom=1.0; }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) { option = 3; centerX= 0.0; zoom=1.0; }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) { option = 4; centerX= 0.0; zoom=1.0; }

        // -----------------------------------------------------
        // COMPUTE PASS (runs fractal iteration)
        // -----------------------------------------------------
        glUseProgram(computeProgram);

        // Upload fractal parameters to compute shader
        GLint locCenter = glGetUniformLocation(computeProgram, "center");
        if (locCenter != -1) {
            double c[2] = { centerX, centerY };
            glUniform2dv(locCenter, 1, c);
        }

        glUniform1d(glGetUniformLocation(computeProgram, "zoom"), zoom);
        glUniform1i(glGetUniformLocation(computeProgram, "option"), option);
        glUniform1i(glGetUniformLocation(computeProgram, "maxIterations"), maxIterations);
        glUniform2i(glGetUniformLocation(computeProgram, "resolution"), WIDTH, HEIGHT);

        // Bind texture as writable image
        glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        // Number of thread groups (16×16 threads per group)
        GLuint gx = (WIDTH  + 15) / 16;
        GLuint gy = (HEIGHT + 15) / 16;

        glDispatchCompute(gx, gy, 1);

        // Ensure compute writes are visible to the fragment shader
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // -----------------------------------------------------
        // RENDER PASS (fullscreen quad)
        // -----------------------------------------------------
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(renderProgram);

        // Bind fractal texture for rendering
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex);
        glUniform1i(glGetUniformLocation(renderProgram, "tex"), 0);

        // Draw quad
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        // Present image to screen
        glfwSwapBuffers(window);
    }

    // -----------------------------------------------------
    // Cleanup resources
    // -----------------------------------------------------
    glDeleteTextures(1, &tex);
    glDeleteProgram(computeProgram);
    glDeleteProgram(renderProgram);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}
