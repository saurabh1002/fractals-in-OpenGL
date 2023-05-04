// Link Statically with GLEW
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <cmath>
#include <iostream>

void setGLFWWindowHints() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

void initGLEW() {
  glewExperimental = GL_TRUE;
  glewInit();
}

// clang-format off
const GLchar *vertexSource = R"glsl(
    #version 150 core
    in vec2 position;
    void main() 
    { 
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

const GLchar *fragmentSource = R"glsl(
    #version 150 core
    out vec4 outColor;
    uniform vec3 triangleColor;
    void main()
    {
        outColor = vec4(triangleColor, 1.0);
    }
)glsl";

// clang-format on

int main() {
  auto t_start = std::chrono::high_resolution_clock::now();

  glfwInit();
  setGLFWWindowHints();

  GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
  glfwMakeContextCurrent(window);
  initGLEW();

  // Create Vertex Array Object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLuint vbo;            // Vertex Buffer Object
  glGenBuffers(1, &vbo); // Generate 1 buffer

  GLfloat vertices[] = {
      0.0f,  0.5f,  1.0f, 0.0f, 0.0f, // Vertex 1 (X, Y) Red
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 1 (X, Y) Green
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 1 (X, Y) Blue
  };

  glBindBuffer(GL_ARRAY_BUFFER, vbo); // Activate Buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW); // Refers to the active buffer internally, no
                                // explicit buffer id given

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  GLint vertexshader_compiled;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexshader_compiled);
  if (!vertexshader_compiled) {
    char buffer[512];
    glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
    std::cout << buffer << "\n";
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);

  GLint fragmentshader_compiled;
  glGetShaderiv(fragmentshader_compiled, GL_COMPILE_STATUS,
                &fragmentshader_compiled);
  if (!fragmentshader_compiled) {
    char buffer[512];
    glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
    std::cout << buffer << "\n";
  }

  // Link the vertex and fragment shader into a shader program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glBindFragDataLocation(shaderProgram, 0, "outColor");
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

  // Specify the layout of the vertex data
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

  while (!glfwWindowShouldClose(window)) {

    auto t_now = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration_cast<std::chrono::duration<float>>(
                     t_now - t_start)
                     .count();

    auto r = (std::sin(time * 4.0f) + 1.0f) / 2.0f;
    glUniform3f(uniColor, r, 1.0f - r, 0.5f * r);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glDeleteProgram(shaderProgram);
  glDeleteShader(fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);

  glfwTerminate();
  return 0;
}