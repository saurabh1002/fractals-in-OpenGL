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
    in vec3 color;
    out vec3 Color;
    void main() 
    { 
        Color = color;
        gl_Position = vec4(position, 0.0, 1.0);
    }
)glsl";

const GLchar *fragmentSource = R"glsl(
    #version 150 core
    in vec3 Color;
    out vec4 outColor;
    void main()
    {
        outColor = vec4(Color, 1.0);
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
      0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // Vertex 1 (X, Y) Red
      -1.0f, 1.0f,  0.0f, 1.0f, 0.0f, // Vertex 2 (X, Y) Green
      1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // Vertex 3 (X, Y) Blue
      1.0f,  -1.0f, 1.0f, 0.0f, 0.0f, // Vertex 4 (X, Y) Green
      -1.0f, -1.0f, 1.0f, 0.0f, 1.0f  // Vertex 5 (X, Y) Blue
  };

  glBindBuffer(GL_ARRAY_BUFFER, vbo); // Activate Buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
               GL_STATIC_DRAW); // Refers to the active buffer internally, no
                                // explicit buffer id given

  GLuint ebo;
  glGenBuffers(1, &ebo);

  GLuint elements[] = {0, 2, 1, 0, 3, 2, 0, 4, 3, 0, 1, 4};

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
               GL_STATIC_DRAW);

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

  // Specify the layout of the vertex data
  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

  GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
  glEnableVertexAttribArray(colAttrib);
  glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(2 * sizeof(float)));

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
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