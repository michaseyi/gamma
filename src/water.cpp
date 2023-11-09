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

Gamma::FlyCam fps_cam(
    glm::vec3(0.0f, 0.0f, 3.0f),
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

float min(float value1, float min)
{
   if (value1 < min)
      return min;
   return value1;
}

glm::vec3 velocity(100.0f, 0.0f, 0.0f);
glm::vec3 down(0.0f, -1.0f, 0.0f);
float radius = 1.0f;
float gravity = 90;
float collision_damping = 0.85;
glm::vec3 box_bound(130.0f, 60.0f, 130.0f);
glm::vec3 molecule_position(0.0f, -29.0f, 0.0f);

void resolve_collision(glm::vec3 &object_position)
{
   glm::vec3 half_bound_size = (box_bound / 2.0f) - (glm::vec3(1.0f) * radius);

   if (glm::abs(object_position.x) > half_bound_size.x)
   {
      object_position.x = half_bound_size.x * glm::sign(object_position.x);
      velocity.x *= -1 * collision_damping;
   }
   if (glm::abs(object_position.y) > half_bound_size.y)
   {
      object_position.y = half_bound_size.y * glm::sign(object_position.y);
      velocity.y *= -1 * collision_damping;
   }
   if (glm::abs(object_position.z) > half_bound_size.z)
   {
      object_position.z = half_bound_size.z * glm::sign(object_position.z);
      velocity.z *= -1 * collision_damping;
   }
}

void update_position(float delta_time, glm::vec3 &current_position)
{
   velocity += down * gravity * delta_time;
   current_position += velocity * delta_time;
   resolve_collision(current_position);
}

glm::mat4 look(glm::vec3 &eye, glm::vec3 &&target, glm::vec3 &up)
{
   glm::mat4 translate_mat{1.0f};
   translate_mat = glm::translate(translate_mat, -eye);
   auto front = glm::normalize(eye - target);
   up -= (glm::dot(up, front) * front);
   up = glm::normalize(up);

   auto side = glm::normalize(glm::cross(up, front));
   glm::mat4 rotate_mat = {
       glm::vec4(side.x, side.y, side.z, 0),
       glm::vec4(up.x, up.y, up.z, 0),
       glm::vec4(front.x, front.y, front.z, 0),
       glm::vec4(0, 0, 0, 1),
   };
   return glm::inverse((rotate_mat)) * translate_mat;
}

int main()
{
   std::cout << sizeof(glm::vec3) << std::endl;
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
   glfwSetErrorCallback([](int a, const char *err)
                        { std::cout << err << std::endl; });
   object_shader.use();

   auto box_texture = Gamma::Texture2D::FromImage("src/assets/steel.jpg", GL_TEXTURE1);
   auto wood_texture = Gamma::Texture2D::FromImagePNG("src/assets/wood_box.png", GL_TEXTURE2);

   // sphere;
   Gamma::VertexArrayObject directions;
   Gamma::Geometry::Arrow arrow{1.0f, 32, 2.0f};
   arrow.bind();
   directions.unbind();

   Gamma::VertexArrayObject sphere;
   Gamma::Geometry::Sphere sphere_geo(2.0f, 128);
   sphere_geo.bind();
   sphere.unbind();

   while (!glfwWindowShouldClose(engine->current_window))
   {
      float time = glfwGetTime();
      float currentFrame = time;
      engine->clear_buffer();

      object_shader.use();
      engine->poll_events();

      time = 0.0f;
      glm::mat4 view(1.0f);
      view = look(fps_cam.pos, fps_cam.pos + fps_cam.front, fps_cam.up);
      // view = glm::lookAt(fps_cam.pos, fps_cam.pos + fps_cam.front, fps_cam.up);

      glm::mat4 projection(1.0f);
      projection = glm::perspective(glm::radians(fov), (engine->width) / (engine->height / 1.0f), 0.1f, 1000.0f);
      auto light_color = glm::vec3(1.0f);
      auto light_position = glm::vec3((cos(time) * 4.0f) + 4.0f, 14.0f, sin(time) * 4.0f);

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
      directions.bind();
      color = glm::vec3(1.0f, 0.0f, 0.0f);
      object_shader.set_vec3("color", color);
      model = glm::mat4(1.0f);
      model = glm::rotate(model, time, rotate_axis);
      object_shader.set_mat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, arrow.vertex_count);

      color = glm::vec3(0.0f, 1.0f, 0.0f);
      object_shader.set_vec3("color", color);
      model = glm::mat4(1.0f);
      model = glm::rotate(model, time, rotate_axis);
      model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      object_shader.set_mat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, arrow.vertex_count);

      color = glm::vec3(0.0f, 0.0f, 1.0f);
      model = glm::mat4(1.0f);
      model = glm::rotate(model, time, rotate_axis);
      model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
      object_shader.set_vec3("color", color);
      object_shader.set_mat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, arrow.vertex_count);

      // light source
      light_shader.use();
      sphere.bind();
      model = glm::mat4(1.0f);
      model = glm::translate(model, light_position);
      model = glm::scale(model, glm::vec3(0.3f));
      light_shader.set_mat4("model", model);
      light_shader.set_mat4("view", view);
      light_shader.set_mat4("projection", projection);
      light_shader.set_vec3("color", light_color);
      glDrawArrays(GL_TRIANGLES, 0, sphere_geo.vertex_count);

      engine->swap_buffer()->run_callbacks();
      delta_time = currentFrame - last_frame;
      last_frame = currentFrame;
   };

   glfwTerminate();
   return EXIT_SUCCESS;
}
