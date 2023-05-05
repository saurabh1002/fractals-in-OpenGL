#include "shader.h"
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int screen_width{1080};
int screen_height{1080};

int num_frames{0};
float last_time{0.0f};

float zoom = 2.0f;
float center_x = 0.75f;
float center_y = 0.5f;

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

void countFPS() {
  double current_time = glfwGetTime();
  num_frames++;
  if (current_time - last_time >= 1.0) {
    std::cout << 1000.0 / num_frames << "ms / frame\n";
    num_frames = 0;
    last_time += 1.0;
  }
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  if (yoffset == -1) {
    zoom *= 1.2f;
  }
  if (yoffset == 1) {
    zoom /= 1.2f;
  }
}

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action,
                      int mods) {
  if (key == GLFW_KEY_R && action == GLFW_RELEASE) {
    center_x = 0.75f;
    center_y = 0.5f;
    zoom = 2.0f;
  }
  if (key == GLFW_KEY_UP && action == GLFW_RELEASE) {
    center_y -= 0.1f;
  }
  if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE) {
    center_y += 0.1f;
  }
  if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
    center_x += 0.1f;
  }
  if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
    center_x -= 0.1f;
  }
}

int main() {
  std::cout << "Use Arrow keys to control the XY position of the Plot"
            << std::endl;
  std::cout << "Use Mouse Scroll Wheel to Zoom In and Out" << std::endl;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(screen_width, screen_height,
                                        "Mandelbrot Set", NULL, NULL);

  if (window == nullptr) {
    std::cout << "Failed to create GLFW window!\n";
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (glewInit()) {
    std::cout << "Failed initializing GLEW\n";
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

  Shader our_shader("/home/ssg1002/dev/fractals/MandelbrotSet/shader.vert",
                    "/home/ssg1002/dev/fractals/MandelbrotSet/shader.frag");

  last_time = glfwGetTime();

  glEnable(GL_DEPTH_TEST);
  our_shader.use_shader();
  GLint fractalCenter = glGetUniformLocation(our_shader.program_ID, "center");
  GLint fractalZoom = glGetUniformLocation(our_shader.program_ID, "zoom");

  glfwSetKeyCallback(window, keyboardCallback);
  glfwSetScrollCallback(window, scrollCallback);

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.0f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    countFPS();

    glUniform2f(fractalCenter, center_x, center_y);
    glUniform1f(fractalZoom, zoom);

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