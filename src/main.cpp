#include "../lib/glad/glad.h"
#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <fstream>
#include "Gamma.h"

void process_input(GLFWwindow *window)
{
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
   {
      glfwSetWindowShouldClose(window, true);
   }
}

std::vector<float> genCircle()
{
   std::vector<float> points;

   float start = 0.0f, end = 2 * M_PI, time;

   while (start <= end)
   {

      points.push_back(cos(start));
      points.push_back(sin(start));

      start += 0.05;
   }
   return points;
}

int main()
{
   Gamma::Engine *engine = Gamma::Engine::New("myengine", 800, 800);
   Gamma::ShaderProgram shader_program("src/shaders/shader.vert", "src/shaders/shader.frag");

   shader_program.use();

   std::vector<float> vertex_data = {
       -0.6f,
       -0.5f,
       1.0f,
       1.0f,
       0.0f,
       0.0f,
       1.0f,

       0.0f,
       0.5f,
       1.0f,
       0.0f,
       1.0f,
       0.0f,
       1.0f,

       0.6f,
       -0.5f,
       1.0f,
       0.0f,
       0.0f,
       1.0f,
       1.0f,
   };

   std::vector<uint> vertex_index = {
       0,
       1,
       2,
   };

   Gamma::VertexArrayObject vao2;
   Gamma::VertexDataBuffer vertex_buffer;
   vertex_buffer.buffer_data(vertex_data);
   vertex_buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, 0);
   vertex_buffer.set_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, sizeof(float) * 3);
   Gamma::VertexElementBuffer element_buffer;
   element_buffer.buffer_data(vertex_index);

   std::vector<float> text_coords = {
       0.0f, 0.0f,
       1.0f, 0.0f,
       0.5f, 1.0f};

   while (!glfwWindowShouldClose(engine->current_window))
   {
      process_input(engine->current_window);
      engine->clear_buffer();

      shader_program.use();
      // shader_program.set_float("time", glfwGetTime());

      vao2.bind();

      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

      engine->swap_buffer()->poll_events();
   };

   glfwTerminate();
   return EXIT_SUCCESS;
}
