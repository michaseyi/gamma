#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include "Gamma.h"
#include <ostream>
#include "Camera.h"

std::vector<float> vertices = {
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
std::vector cubePositions = {

    glm::vec3(0.0f, 0.0f,
              0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)};
std::vector<float> plane_vertices_data = {
       -10.0f,
       0.0f,
       -10.0f,
       -10.0f,
       0.0f,
       10.0f,
       10.0f,
       0.0f,
       10.0f,
       10.0f,
       0.0f,
       -10.0f,
   };
std::vector<uint> plane_element_object_index = {
       0,
       1,
       2,
       0,
       3,
       2,
   };

Gamma::FPSCam fps_cam(
    glm::vec3(0.0f, 3.0f, 20.0f),
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    24.0f);
    
Gamma::Engine *engine = Gamma::Engine::New("3D Stuff", 1.0f, 800);
Gamma::Joystick joystick(GLFW_JOYSTICK_1);
float fov = 45.0f;
float delta_time = 0.0f;
float last_frame = 0.0f;



void process_input(GLFWwindow *window)
{
   if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
   Gamma::BaseCamera::update_from_joystick(fps_cam, joystick);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
   fov -= (float)yoffset;
   if (fov < 1.0f)
      fov = 1.0f;
   if (fov > 45.0f)
      fov = 45.0f;
};

int main()
{
   fps_cam.delta_time = &delta_time;
   glfwSetFramebufferSizeCallback(engine->current_window, [](GLFWwindow *window, int width, int height)
                                  {
                                     engine->width = width;
                                     engine->height = height;
                                     glViewport(0, 0, width, height); });

   Gamma::ShaderProgram cube_shader("src/shaders/shader.vert", "src/shaders/shader.frag");
   Gamma::ShaderProgram plane_shader("src/shaders/grid.vert", "src/shaders/grid.frag");
   cube_shader.use();
   // zooming in and out
   glfwSetScrollCallback(engine->current_window, scroll_callback);
   Gamma::VertexArrayObject cube_array_object;
   Gamma::Texture2D texture = Gamma::Texture2D::FromImage("src/assets/box_two.jpg");
   Gamma::VertexDataBuffer vertex_buffer;
   vertex_buffer.buffer_data(vertices);
   vertex_buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
   vertex_buffer.set_attrib_pointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);
   engine->set_callback(process_input);
   Gamma::VertexArrayObject plane_array_object;
   Gamma::VertexDataBuffer plane_array_buffer;
   
   plane_array_buffer.buffer_data(plane_vertices_data);
   plane_array_buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
   Gamma::VertexElementBuffer plane_element_object;
   
   plane_element_object.buffer_data(plane_element_object_index);

   while (!glfwWindowShouldClose(engine->current_window))
   {
      float currentFrame = glfwGetTime();
      engine->clear_buffer();
      cube_shader.use();
      engine->poll_events();
      cube_array_object.bind();
      glm::mat4 view(1.0f);
      view = glm::lookAt(fps_cam.pos, fps_cam.pos + fps_cam.front, fps_cam.up);
      glm::mat4 projection(1.0f);
      projection = glm::perspective(glm::radians(fov), engine->width / (engine->height / 1.0f), 0.1f, 100.0f);
      glm::mat4 model(1.0f);
      cube_shader.set_mat4("view", view);
      cube_shader.set_mat4("projection", projection);
      for (auto &position : cubePositions)
      {
         model = glm::mat4(1.0f);
         position.y = 0.5f;
         model = glm::translate(model, position);
         cube_shader.set_mat4("model", model);
         glDrawArrays(GL_TRIANGLES, 0, 36);
      }
      plane_shader.use();
      plane_array_object.bind();
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, -7.0f));
      plane_shader.set_mat4("model", model);
      plane_shader.set_mat4("view", view);
      plane_shader.set_mat4("projection", projection);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      engine->swap_buffer()->run_callbacks();
      delta_time = currentFrame - last_frame;
      last_frame = currentFrame;
   };

   glfwTerminate();
   return EXIT_SUCCESS;
}
