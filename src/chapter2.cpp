#include <cmath>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include "Gamma.h"
#include <ostream>
#include "Camera.h"
#include "Geometry.h"

std::vector<float> vertices = {
    // positions          // normals           // plane_texture coords
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f};

std::vector<float> plane_vertices_data = {
    -10.0f,
    0.0f,
    -10.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,
    4.0f,

    -10.0f,
    0.0f,
    10.0f,
    0.0f,
    1.0f,
    0.0f,
    0.0f,
    0.0f,

    10.0f,
    0.0f,
    10.0f,
    0.0f,
    1.0f,
    0.0f,
    4.0f,
    0.0f,

    10.0f,
    0.0f,
    -10.0f,
    0.0f,
    1.0f,
    0.0f,
    4.0f,
    4.0f};

std::vector<uint> plane_element_object_index = {
    0,
    2,
    3,
    1,
    0,
    2,
};

Gamma::FlyCam fps_cam(
    glm::vec3(0.0f, 3.0f, 20.0f),
    glm::vec3(0.0f, 0.0f, -1.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    24.0f);

Gamma::Engine *engine = Gamma::Engine::New("3D Stuff", 1.0f, 800);
Gamma::Joystick joystick(GLFW_JOYSTICK_2);
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

float min(float value1, float min)
{
   if (value1 < min)
      return min;
   return value1;
}

int main()
{
   fps_cam.delta_time = &delta_time;

   engine->set_callback(process_input);
   // registering callbacks
   glfwSetScrollCallback(engine->current_window, scroll_callback);
   glfwSetFramebufferSizeCallback(engine->current_window, [](GLFWwindow *window, int width, int height)
                                  {
                                     engine->width = width;
                                     engine->height = height;
                                     glViewport(0, 0, width, height); });

   Gamma::ShaderProgram object_shader("src/shaders/shader_2.vert", "src/shaders/shader_2.frag");
   Gamma::ShaderProgram light_shader("src/shaders/shader_2.vert", "src/shaders/shader_2_light.frag");
   object_shader.use();

   // box
   Gamma::VertexArrayObject cube_array_object;
   Gamma::VertexDataBuffer vertex_buffer;
   vertex_buffer.buffer_data(vertices);
   vertex_buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
   vertex_buffer.set_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
   vertex_buffer.set_attrib_pointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

   // plane;
   Gamma::VertexArrayObject plane_array_object;
   Gamma::VertexDataBuffer plane_array_buffer;
   plane_array_buffer.buffer_data(plane_vertices_data);
   plane_array_buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
   plane_array_buffer.set_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
   plane_array_buffer.set_attrib_pointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);
   Gamma::VertexElementBuffer plane_element_object;
   plane_element_object.buffer_data(plane_element_object_index);

   auto plane_texture = Gamma::Texture2D::FromImage("src/assets/chess_floor.jpg", GL_TEXTURE0);
   auto box_texture = Gamma::Texture2D::FromImage("src/assets/steel.jpg", GL_TEXTURE1);
   auto wood_texture = Gamma::Texture2D::FromImagePNG("src/assets/wood_box.png", GL_TEXTURE2);
   auto floor_texture = Gamma::Texture2D::FromImage("src/assets/tiles.jpg", GL_TEXTURE3);
   auto white_texture = Gamma::Texture2D::FromImagePNG("src/assets/white.png", GL_TEXTURE4);

   //sphere;
   // Gamma::VertexArrayObject sphere_array;
   // Gamma::Sphere sphere;
   

   while (!glfwWindowShouldClose(engine->current_window))
   {
      float currentFrame = glfwGetTime();
      engine->clear_buffer();
      object_shader.use();
      engine->poll_events();
      cube_array_object.bind();
      float time = glfwGetTime();

      glm::mat4 view(1.0f);
      view = glm::lookAt(fps_cam.pos, fps_cam.pos + fps_cam.front, fps_cam.up);
      glm::mat4 projection(1.0f);
      projection = glm::perspective(glm::radians(fov), (engine->width) / (engine->height / 1.0f), 0.1f, 1000.0f);

      auto box_color = glm::vec3(0.1f, 0.3f, 0.2f);
      auto box_color_small = glm::vec3(0.6f, 0.1f, 0.9f);
      auto plane_color = glm::vec3(0.1f, 0.3f, 0.2f);

      auto light_color = glm::vec3(1.0f);
      auto light_position = glm::vec3((cos(time) * 4.0f) + 4.0f, 14.0f, sin(time) * 4.0f );
      // light_position = glm::vec3(0.0f, 9.0f, 3.0f);
      object_shader.set_vec3("light_pos", light_position);
      object_shader.set_vec3("camera_pos", fps_cam.pos);

      glm::vec3 light_ambient(0.2f);
      glm::vec3 light_diffuse(0.5f);
      glm::vec3 light_spec(1.0f);

      object_shader.set_vec3("light.ambient", light_ambient);
      object_shader.set_vec3("light.diffuse", light_diffuse);
      object_shader.set_vec3("light.specular", light_spec);

      object_shader.set_vec3("light.position", light_position);
      object_shader.set_float("light.constant", 1.0f);
      object_shader.set_float("light.linear", 0.027f);
      object_shader.set_float("light.quadratic", 0.0028f);

      // big box
      glm::mat4 model(1.0f);
      model = glm::translate(model, glm::vec3(10.0f, 4.51, -4.0f));
      model = glm::rotate(model, 3 * time, glm::vec3(2.0f, 1.0f, 3.1f));
      model = glm::scale(model, glm::vec3(5.0f));
      object_shader.set_mat4("model", model);
      object_shader.set_mat4("view", view);
      object_shader.set_mat4("projection", projection);
      glm::vec3 specular(0.5f);
      object_shader.set_int("material.diffuse", 1);
      object_shader.set_vec3("material.specular", specular);
      object_shader.set_float("material.shininess", 32.0f);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // small box
      object_shader.set_int("material.diffuse", 2);
      object_shader.set_vec3("material.specular", specular);
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 4.51f, -3.0f));
      model = glm::rotate(model, 3 * time, glm::vec3(5.0f, 8.0f, 3.1f));
      model = glm::scale(model, glm::vec3(5.0f));
      object_shader.set_mat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

     
      // plane
      object_shader.set_int("material.diffuse", 4);
      object_shader.set_float("material.shininess", 32.0f);
      plane_array_object.bind();
      specular = glm::vec3(0.2f);
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
      model = glm::scale(model, glm::vec3(4.0f));
      object_shader.set_vec3("material.specular", specular);
      object_shader.set_mat4("model", model);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      // light cube
      light_shader.use();
      cube_array_object.bind();
      model = glm::mat4(1.0f);
      model = glm::translate(model, light_position);
      model = glm::scale(model, glm::vec3(0.3f));
      light_shader.set_mat4("model", model);
      light_shader.set_mat4("view", view);
      light_shader.set_mat4("projection", projection);
      light_shader.set_vec3("color", light_color);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      engine->swap_buffer()->run_callbacks();
      delta_time = currentFrame - last_frame;
      last_frame = currentFrame;
   };

   glfwTerminate();
   return EXIT_SUCCESS;
}
