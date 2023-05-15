#version 330 core

in vec4 gl_FragCoord;
out vec4 frag_color;

uniform vec2 screen_dimension;
uniform vec2 complex_constant;
uniform vec2 center;
uniform float zoom;
uniform int symmetry;

#define MAX_ITERATIONS 100

int get_iterations()
{
    float real = (gl_FragCoord.x / screen_dimension.x - center.x) * zoom;
    float imag = (gl_FragCoord.y / screen_dimension.y - center.y) * zoom;

    int iterations = 0;

    while(iterations < MAX_ITERATIONS)
    {
        float temp_real = real;
        
        // z^2 + c - Two way symmetry
        if (symmetry == 2)
        {
            real = (real * real - imag * imag) + complex_constant.x;
            imag = (2.0 * temp_real * imag) + complex_constant.y;
        }

        // z^3 + c - Three way symmetry
        else if(symmetry == 3)
        {
            real = real * (real * real - imag * imag) - (2.0 * real * imag * imag) + complex_constant.x;
            imag = imag * (temp_real * temp_real - imag * imag) + (2.0 * temp_real * temp_real * imag) + complex_constant.y;
        }

        float dist = real * real + imag * imag;
        if (dist >= 10.0)
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

    switch (int(iter / 3)){
        case 0:
            return vec4(253.0f / 255.0f, 0.0f, 1.0f, 1.0f);
        case 1:
            return vec4(253.0f / 255.0f, 1.0f, 0.0f, 1.0f);
        case 2:
            return vec4(0.0f, 1.0f, 56.0f / 255.0f, 1.0f);
        case 3:
            return vec4(0.0f, 249.0f / 255.0f, 1.0f, 1.0f);
        case 4:
            return vec4(60.0f / 255.0f, 0.0f, 1.0f, 1.0f);
        default:
            return vec4(0.0f, 1.0f, 0.0f, 1.0f);
    }
}

void main()
{
    frag_color = return_color();
}