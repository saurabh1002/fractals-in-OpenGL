set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(shader STATIC shader.cpp)
target_link_libraries(shader PUBLIC glfw GLEW GL)

add_executable(mandelbrot main.cpp)
target_link_libraries(mandelbrot PUBLIC shader glfw GLEW GL)