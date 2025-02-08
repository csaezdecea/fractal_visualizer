#version 330 core
in vec4 gl_FragCoord;
 
out vec4 frag_color;
uniform vec4 ShaderVars;
int option = int(ShaderVars.w);

#define MAX_ITERATIONS 400
 
int get_iterations(int op) {
    float zoom = ShaderVars.z;
    float center_x = ShaderVars.x;
    float center_y = ShaderVars.y;
    float real = ((gl_FragCoord.x / 1600.0 - 0.5) * zoom + center_x) * 5.0;
    float imag = ((gl_FragCoord.y / 1600.0 - 0.5) * zoom + center_y) * 5.0;
    int iterations = 0;
    float const_real = real;
    float const_imag = imag;
    if (op == 1) {
        while (iterations < MAX_ITERATIONS) {
            float tmp_real = real;
            real = (real * real - imag * imag) + const_real;
            imag = (2.0 * tmp_real * imag) + const_imag;
            float dist = real * real + imag * imag;
            if (dist > 4.0) break;
            ++iterations;
        }
    }
    if (op == 2) {
        while (iterations < MAX_ITERATIONS) {
            float tmp_real = real;
            real = exp(real)*cos(imag)-0.65;
            imag = exp(tmp_real)*sin(imag);
            float dist = log(real * real + imag * imag);
            if (dist > 8.0) break;
            ++iterations;
        }
    }
       if (op == 3) {
        while (iterations < MAX_ITERATIONS) {
            float tmp_real = real;
            real = (real * real - imag * imag) + 0.279;
            imag = (2.0 * tmp_real * imag);
            float dist = real * real + imag * imag;
            if (dist > 4.0) break;
            ++iterations;
        }
    }

   if (op == 4) {
        while (iterations < MAX_ITERATIONS) {
            float tmp_real = real;
            real = exp(real)*(const_real*cos(imag)-const_imag*sin(imag));
            imag = exp(tmp_real)*(const_real*sin(imag)+const_imag*cos(imag));
            float dist = log(real * real + imag * imag);
            if (dist > 8.0) break;
            ++iterations;
        }
   }
   return iterations;
}
   
 
vec4 return_color()
{
    float iter = float(get_iterations(option));
    if (iter == MAX_ITERATIONS)
    {
        gl_FragDepth = 0.0f;
        return vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    float maxa;
    if (option == 1) maxa = 120.0;
    if (option == 2) maxa = 20.0;
    if (option == 3) maxa = 40;   
    if (option == 4) maxa = 20.0;
    float ita = float(iter)/ maxa;
    if (iter < maxa) {
        return vec4(0.0f,ita, 0.0f, 1.0f);
        } else return vec4(0.1f,0.4f,0.4f,1.0f);
} 

 
void main()
{
    frag_color = return_color();
}