#version 330 core

in vec4 gl_FragCoord;
out vec4 frag_color;

uniform vec2 center;
uniform float zoom;

#define MAX_ITERATIONS 500

int get_iterations()
{
    float real = (gl_FragCoord.x / 1080.0 - center.x) * zoom;
    float imag = (gl_FragCoord.y / 1080.0 - center.y) * zoom;

    int iterations = 0;
    float const_real = real;
    float const_imag = imag;

    while(iterations < MAX_ITERATIONS)
    {
        float temp_real = real;
        real = (real * real - imag * imag) + const_real;
        imag = (2.0 * temp_real * imag) + const_imag;

        float dist = real * real + imag * imag;
        if (dist >= 2.0)
        {
            break;
        }
        ++iterations;
    }
    return iterations;
}

vec4 return_color()
{
    int iter = get_iterations();
    if (iter == MAX_ITERATIONS)
    {
        gl_FragDepth = 0.0f;
        return vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    float iterations = float(iter) / MAX_ITERATIONS * 5.0;
    return vec4(0.0f, iterations, iterations, 1.0f);
}

void main()
{
    frag_color = return_color();
}