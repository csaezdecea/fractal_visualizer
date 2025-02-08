He aqui los comandos posibles:

tecla        |   resultado
---------------------
z            | zoom in
x            | zoom out
right_arrow  | mueve a la derecha
left_arrow   | mueve a la izquierda
up_arrow     | mueve hacia arriba
down_arrow   | mueve hacia abajo
1            | set 1*
2            | set 2*
3            | set 3*
4            | set 4*
ESC          | exit/quit

*Todos los set estan descritos por _z_{n+1} = f(z_n)
- set 1: set de Mandelbrot f(z) = z_n^2 + c
- set 2: f(z) = exp(z) - 0.65
- set 3: f(z) = z^2 + 0.279           
- set 4: f(z) = c * exp(z)

El programa se compila con el makefile

