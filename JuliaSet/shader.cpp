#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "shader.h"

std::string Shader::read_shader_file(const std::string &file_path) {
  std::string code;
  std::ifstream shader_file(file_path, std::ios::in);

  if (!shader_file) {
    std::cout << "Failed to open shader file: " << file_path << "\n";
    return "";
  }

  std::string line = "";
  while (std::getline(shader_file, line)) {
    code.append(line + '\n');
  }

  shader_file.close();
  return code;
}

void Shader::add_shader(unsigned int program, const std::string &shader_path,
                        GLenum shader_type) {
  std::string shader_string = read_shader_file(shader_path);

  const GLchar *code[1];
  code[0] = shader_string.c_str();

  GLint code_length[1];
  code_length[0] = strlen(shader_string.c_str());

  unsigned int shader;
  int success;
  char error_message[512];

  shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, code, code_length);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, error_message);
    std::cout << "Error compiling shader: " << error_message << "\n";
    std::cout << "Shader location: " << shader_path << "\n";
  }

  glAttachShader(program, shader);
}

Shader::Shader(const std::string &vertex_shader_path,
               const std::string &frag_shader_path) {
  program_ID = glCreateProgram();

  add_shader(program_ID, vertex_shader_path, GL_VERTEX_SHADER);
  add_shader(program_ID, frag_shader_path, GL_FRAGMENT_SHADER);

  glLinkProgram(program_ID);

  int success;
  char error_message[512];
  glGetProgramiv(program_ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program_ID, 512, nullptr, error_message);
    std::cout << "Error linking shader program: " << error_message << "\n";
  }
}

Shader::~Shader() {
  if (program_ID != 0) {
    glDeleteProgram(program_ID);
    program_ID = 0;
  }
}

void Shader::use_shader() const { glUseProgram(program_ID); }

void Shader::set_float(const std::string &name, const float value) const {
  glUniform1f(glGetUniformLocation(program_ID, name.c_str()), value);
}

void Shader::set_vec4(const std::string &name, const glm::vec4 vec) const {
  glUniform4f(glGetUniformLocation(program_ID, name.c_str()), vec.x, vec.y,
              vec.z, vec.w);
}