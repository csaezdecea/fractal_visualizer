# Description  
This is an OpenGL program for visualizing fractal sets using the GPU. A demo and a brief description are available in Spanish in the attached video.  

# Available Commands  
Once you run the program with:  
```sh
./main
```

## key Bindings

| Tecla        | Resultado             |
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

### Fractal sets shown:
- **Set 1:** Conjunto de Mandelbrot, \( f(z) = z_n^2 + c \)
- **Set 2:** \( f(z) = \exp(z) - 0.65 \)
- **Set 3:** \( f(z) = z^2 + 0.279 \)
- **Set 4:** \( f(z) = c \cdot \exp(z) \)

## Compilation

To compile the program, use:
```
make
```
To clean previous compilations, use:
```
make clean
```