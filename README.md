# Fractal Explorer (OpenGL + Compute Shader)

This is an interactive GPU-accelerated (OpenGL) program for visualizing fractal sets. A demo and a brief description are available in Spanish in the attached video.  This software is  written
in **C++**, using:

-   **OpenGL 4.3+**
-   **GLFW** (window + input)
-   **GLEW** (extensions)
-   **Compute shaders** for high-resolution fractal rendering
-   **Vertex/fragment shaders** for fullscreen rendering

It supports: - Window resizing - Real-time zooming - Panning with arrow
keys - Switching between multiple fractal families

------------------------------------------------------------------------

## 🎯 Features

### ✔ Real-time rendering using the GPU

Each frame is generated using a compute shader that writes directly into
an `RGBA32F` texture.

### ✔ Keyboard navigation

|    Key      |     Output           |
|-------------|----------------------|
| `z`         | Zoom in              |
| `x`         | Zoom out             |
| `→` (right_arrow)  | Move to the right  |
| `←` (left_arrow)   | Move to the left |
| `↑` (up_arrow)     | Move upward  |
| `↓` (down_arrow)   | Move downward   |
| `1`         | Set 1*               |
| `2`         | Set 2*               |
| `3`         | Set 3*               |
| `4`         | Set 4*               |
| `ESC`       | Exit/Quit            |

\* All the fractal sets are descrived by the equation _z<sub>n+1</sub> = f(z<sub>n</sub>)_

**Fractal sets shown:**

- **Set 1:**  $~~~~f(z) = z^2 + c~~~~$ (Mandelbrot set)

- **Set 2:**  $~~~~f(z) = \exp(z) - 0.65$

- **Set 3:**  $~~~~f(z) = z^2 + 0.279$

- **Set 4:**  $~~~~f(z) = c \cdot \exp(z)$

### ✔ Automatic resolution adaptation

When the window is resized, a new texture of the appropriate size is
created.

### ✔ Double-precision Mandelbrot & Julia

Allows deep zoom levels with little numerical drift.

------------------------------------------------------------------------

## 🧠 Main Architecture Overview

## 1. Helper functions

### fractal_utils.hpp

- `framebuffer_size_callback` GLFW callback that updates the OpenGL viewport when the window is resized.
- `readFile` for loading shader source files.
- `compileShader` to load and compile vertex, fragment, or compute
shaders.
- `createProgram` to link a vertex + fragment shader program.
- `createCompute` to link a compute shader program.

### fractal_utils.cpp

Implements the above: - Handles framebuffer resize. - Reads files via
`ifstream`. - Compiles shaders and prints errors on failure. - Links
graphics and compute programs, deleting shaders after linking.

### 2. **main.cpp**

Handles window creation, input, texture creation, and dispatching the
compute shader.

#### Key snippet: compute shader dispatch

``` cpp
glUseProgram(computeProgram);

glUniform2dv(glGetUniformLocation(computeProgram, "center"), 1, c);
glUniform1d(glGetUniformLocation(computeProgram, "zoom"), zoom);
glUniform1i(glGetUniformLocation(computeProgram, "option"), option);
glUniform1i(glGetUniformLocation(computeProgram, "maxIterations"), maxIterations);
glUniform2i(glGetUniformLocation(computeProgram, "resolution"), WIDTH, HEIGHT);

glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
glDispatchCompute((WIDTH+15)/16, (HEIGHT+15)/16, 1);
glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
```

------------------------------------------------------------------------

### 3. **shader.comp** --- Compute Shader

This shader computes the fractal at each pixel.\
It supports:

-   Double-precision Mandelbrot
-   Double-precision Julia
-   Float exponential variants

#### Key snippet: Mandelbrot iteration

``` glsl
int computeDoubleMandelbrot(double cr, double ci, int maxI) {
    double zr = 0.0, zi = 0.0;
    int iter = 0;

    while (iter < maxI) {
        double tmp = zr;
        zr = zr*zr - zi*zi + cr;
        zi = 2.0 * tmp * zi + ci;
        if (zr*zr + zi*zi > 4.0) break;
        iter++;
    }
    return iter;
}
```

> **Note:**  
> `maxI` (maximum iterations) controls the level of fractal detail.  
> Higher values give sharper boundaries and more detail but increase render time.  
> Lower values render faster but reduce detail and produce more color banding.  
> Typical range: 300–800 for normal views, 1000+ for zooms.

------------------------------------------------------------------------

### 4. **shader.vert** --- Fullscreen Quad Vertex Shader

``` glsl
#version 430 core
layout(location=0) in vec2 pos;
layout(location=1) in vec2 uvIn;
out vec2 uv;
void main() {
    uv = uvIn;
    gl_Position = vec4(pos, 0.0, 1.0);
}
```

------------------------------------------------------------------------

### 5. **shader.frag** --- Fragment Shader

Simply samples the texture produced by the compute shader.

``` glsl
#version 430 core
in vec2 uv;
out vec4 frag_color;
uniform sampler2D tex;
void main() {
    frag_color = texture(tex, uv);
}
```

------------------------------------------------------------------------

## 🚀 Running the Program

Compile with:

``` bash
mkdir build
cd build
cmake ..
make
```

Then run:

``` bash
./fractal
```

------------------------------------------------------------------------

## 📦 Files in This Project

```
.
├── build
├── CMakeLists.txt
├── Readme.md    <-- this file
├── src
    ├── fractal_utils.cpp
    ├── fractal_utils.hpp
    ├── main.cpp
    └── shaders
        ├── shader.comp
        ├── shader.frag
        └── shader.vert
```
------------------------------------------------------------------------

## 📝 Final Notes

This setup gives you: - GPU-accelerated rendering
- Stable deep zoom
- Flexible input controls
- A clean codebase structured for experimentation

