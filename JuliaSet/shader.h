#pragma once
#define GLEW_STATIC

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader {
public:
  unsigned int program_ID;
  Shader(const std::string &vertex_shader_path,
         const std::string &frag_shader_path);
  ~Shader();

  void use_shader() const;

  void set_float(const std::string &name, const float value) const;
  void set_vec4(const std::string &name, const glm::vec4 vec) const;

private:
  std::string read_shader_file(const std::string &file_path);
  void add_shader(unsigned int program, const std::string &shader_path,
                  GLenum shader_type);
};