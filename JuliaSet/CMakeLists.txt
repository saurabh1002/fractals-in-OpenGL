set(CMAKE_CXX_STANDARD  17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(shader STATIC shader.cpp)
target_link_libraries(shader PUBLIC glfw GLEW GL)

add_executable(julia_animation trippy_animation.cpp)
target_link_libraries(julia_animation PUBLIC shader glfw GLEW GL)

add_executable(julia_play_around play_around.cpp)
target_link_libraries(julia_play_around PUBLIC shader glfw GLEW GL)