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

// Gamma::FlyCam fps_cam(
//     glm::vec3(0.0f, 0.0f, 3.0f),
//     glm::vec3(0.0f, 0.0f, -1.0f),
//     glm::vec3(0.0f, 1.0f, 0.0f),
//     24.0f);

Gamma::FlyCam fps_cam(
    glm::vec3(-11.794, 20.305, 23.317),
    glm::vec3(0.401, -0.518, -0.755),
    glm::vec3(0.0f, 1.0f, 0.0f),
    20.0f);
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

int main()
{
   fps_cam.yaw = -61.992f;
   fps_cam.pitch = -31.224f;
   fps_cam.delta_time = &delta_time;
   engine->set_callback(process_input);
   // registering callbacks
   glfwSetFramebufferSizeCallback(engine->current_window, [](GLFWwindow *window, int width, int height)
                                  {
                                     engine->width = width;
                                     engine->height = height;
                                     glViewport(0, 0, width, height); });

   Gamma::ShaderProgram object_shader("src/shaders/shader_2.vert", "src/shaders/water.frag");

   Gamma::ShaderProgram light_shader("src/shaders/shader_2.vert", "src/shaders/shader_2_light.frag");

   Gamma::Geometry::Arrow arrow{0.1f, 64, 5.0f};
   Gamma::Geometry::Sphere sphere(4.0f, 256);
   Gamma::Geometry::Mat mat{};
   Gamma::Geometry::Donut donut{1.2f, 128};
   while (!glfwWindowShouldClose(engine->current_window))
   {
      float time = glfwGetTime();
      float currentFrame = time;
      engine->clear_buffer();

      object_shader.use();
      engine->poll_events();

      glm::mat4 view(1.0f);
      glm::mat4 projection(1.0f);
      view = glm::lookAt(fps_cam.pos, fps_cam.pos + fps_cam.front, fps_cam.up);
      projection = glm::perspective(glm::radians(fov), (engine->width) / (engine->height / 1.0f), 0.1f, 1000.0f);
      auto light_color = glm::vec3(1.0f);
      auto light_position = glm::vec3((cos(0) * 4.0f) + 4.0f, 14.0f, sin(0) * 4.0f);

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
      object_shader.set_float("light.linear", 0.0014f);
      object_shader.set_float("light.quadratic", 0.000007f);

      
      glm::mat4 model(1.0f);
      glm::vec3 specular(0.5f);

      object_shader.set_mat4("view", view);
      object_shader.set_mat4("projection", projection);

      glm::vec3 rotate_axis(1.0f, 0.8f, 0.3f);

      object_shader.set_int("material.diffuse", 2);
      object_shader.set_vec3("material.specular", specular);
      object_shader.set_float("material.shininess", 17.0f);
      // directions
      auto color = glm::vec3(0.0f);
      color = glm::vec3(1.0f, 0.0f, 0.0f);
      object_shader.set_vec3("color", color);
      model = glm::mat4(1.0f);
      object_shader.set_mat4("model", model);
      arrow.draw();

      color = glm::vec3(0.0f, 1.0f, 0.0f);
      object_shader.set_vec3("color", color);
      model = glm::mat4(1.0f);
      model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      object_shader.set_mat4("model", model);
      arrow.draw();

      color = glm::vec3(0.0f, 0.0f, 1.0f);
      model = glm::mat4(1.0f);
      model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
      object_shader.set_vec3("color", color);
      object_shader.set_mat4("model", model);
      arrow.draw();

      color = glm::vec3(0.8f, 0.8f, 0.8f);
      model = glm::mat4(1.0f);
      object_shader.set_vec3("color", color);
      object_shader.set_mat4("model", model);
      mat.draw();


      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(10.0f, 5.0f, 4.0f));
      model = glm::rotate(model, time * 2, glm::vec3(1.0f, 2.0f, 1.4f));
      model = glm::scale(model, glm::vec3(2.0f));
      object_shader.set_mat4("model", model);
      donut.draw();

      model = glm::mat4(1.0f);
      // model = glm::rotate(model, time * 2, glm::vec3(1.0f, 2.0f, 1.4f));
      model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0f));
      object_shader.set_mat4("model", model);
      color = glm::vec3(0.3f, 1.9f, 0.8f);
      object_shader.set_vec3("color", color);
      sphere.draw();

      engine->swap_buffer()->run_callbacks();
      delta_time = currentFrame - last_frame;
      last_frame = currentFrame;
   };

   glfwTerminate();
   return EXIT_SUCCESS;
}
