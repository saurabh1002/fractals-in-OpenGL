#include "shader.h"

#include <filesystem>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

int screen_width{1080};
int screen_height{1080};

int symmetry{2};
float default_zoom{2.0f};
float default_center_x{0.5f};
float default_center_y{0.5f};

float complex_constant_x{0.15f};
float complex_constant_y{-0.06f};

float vertices[] = {
    -1.0f, -1.0f, -0.0f, // 1
    1.0f,  1.0f,  -0.0f, // 2
    -1.0f, 1.0f,  -0.0f, // 3
    1.0f,  -1.0f, -0.0f  // 4
};

unsigned int indices[] = {
    0, 1, 2, // 1
    0, 3, 1  // 2
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mousebuttonCallback(GLFWwindow *window, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    complex_constant_x = ((xpos / screen_width) - 0.5) * 3.0;
    complex_constant_y = ((ypos / screen_height) - 0.5) * 3.0;
    std::cout << "Julia Set Complex Constant set to (" << complex_constant_x
              << ") + (" << complex_constant_y << "i)\n";
  }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  if (yoffset == -1) {
    default_zoom *= 1.1f;
  }
  if (yoffset == 1) {
    default_zoom /= 1.1f;
  }
}

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods) {
  if (action == GLFW_RELEASE) {
    switch (key) {
    case GLFW_KEY_2:
      symmetry = 2;
      return;

    case GLFW_KEY_3:
      symmetry = 3;
      return;

    case GLFW_KEY_R:
      default_center_x = 0.5f;
      default_center_y = 0.5f;
      default_zoom = 2.0f;
      return;

    case GLFW_KEY_UP:
      default_center_y -= 0.05f;
      return;

    case GLFW_KEY_DOWN:
      default_center_y += 0.05f;
      return;

    case GLFW_KEY_LEFT:
      default_center_x += 0.05f;
      return;

    case GLFW_KEY_RIGHT:
      default_center_x -= 0.05f;
      return;
    }
  }
}

int main() {
  std::cout << "Controls:\n";
  std::cout << "\t[Arrow Keys]\t:\tX, Y position of the plot\n";
  std::cout << "\t[Mouse Scroll]\t:\tZoom in/Zoom out\n";
  std::cout << "\t[Left Click]\t:\tSelect a scaled coordinate in [-1.5, 1.5] "
               "for the complex constant of the Julia Set\n";
  std::cout << "\t[2]\t\t:\tTwo-way symmetric fractal\n";
  std::cout << "\t[3]\t\t:\tThree-way symmetric fractal\n";

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(screen_width, screen_height,
                                        "Julia Set - Playground", NULL, NULL);

  if (window == nullptr) {
    std::cout << "Failed to Create GLFW Window!\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (glewInit()) {
    std::cout << "Failed Initializing GLEW\n";
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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO);

  Shader our_shader(
      std::filesystem::current_path().parent_path() / "shader.vert",
      std::filesystem::current_path().parent_path() / "shader.frag");

  glEnable(GL_DEPTH_TEST);
  our_shader.use_shader();

  GLint screenDimensions =
      glGetUniformLocation(our_shader.program_ID, "screen_dimension");
  GLint fractalCenter = glGetUniformLocation(our_shader.program_ID, "center");
  GLint fractalZoom = glGetUniformLocation(our_shader.program_ID, "zoom");

  GLint fractalConstant =
      glGetUniformLocation(our_shader.program_ID, "complex_constant");
  GLint fractalSymmetry =
      glGetUniformLocation(our_shader.program_ID, "symmetry");

  glfwSetKeyCallback(window, keyboardCallback);
  glfwSetScrollCallback(window, scrollCallback);
  glfwSetMouseButtonCallback(window, mousebuttonCallback);

  while (!glfwWindowShouldClose(window)) {

    glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniform2f(screenDimensions, screen_width, screen_height);
    glUniform2f(fractalCenter, default_center_x, default_center_y);
    glUniform1f(fractalZoom, default_zoom);

    glUniform2f(fractalConstant, complex_constant_x, complex_constant_y);
    glUniform1i(fractalSymmetry, symmetry);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glfwTerminate();

  return 0;
}