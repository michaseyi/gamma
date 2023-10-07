#ifndef __GAMMA__
#define __GAMMA__
#include "lib/glad/glad.h"
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


float round_float(float value);
namespace Gamma
{
   typedef void (*CallbackFunc)(GLFWwindow *);
   class Engine
   {
      std::string app_name;
      std::vector<CallbackFunc> callbacks;

   public:
      uint width;
      uint height;
      float aspect_ratio;
      GLFWwindow *current_window;
      static Engine *New(std::string app_name, float aspect_ratio, uint width);
      void clear_buffer();
      Engine *swap_buffer();
      Engine *poll_events();
      Engine *set_callback(CallbackFunc callback_func);
      void run_callbacks();
   };

   class ShaderProgram
   {

   public:
      uint id;
      ShaderProgram(std::string vert_path, std::string frag_path);
      void use();
      int get_uniform_location(std::string uniform_name);
      void set_bool(std::string name, bool value);
      void set_int(std::string name, int value);
      void set_float(std::string name, float value);
      void set_mat4(std::string nam, glm::mat4 &value);
   };

   class VertexDataBuffer
   {
   public:
      uint id;

      VertexDataBuffer();
      void bind();
      void unbind();
      template <typename T>
      void buffer_data(std::vector<T> &vertices);
      void set_attrib_pointer(uint location, uint length, uint data_type, bool normalize, uint stride, uint64_t offset);
      void enable_attrib_pointer(uint location);
   };

   class VertexElementBuffer
   {
   public:
      uint id;

      VertexElementBuffer();
      void bind();
      void unbind();
      template <typename T>
      void buffer_data(std::vector<T> &indexes);
   };

   class VertexArrayObject
   {
   public:
      uint id;
      VertexArrayObject();
      void bind();
      void unbind();
   };

   class ImageData
   {

   public:
      int width;
      int height;
      int n_channels;
      unsigned char *data;
      ImageData(std::string data_path);

      ~ImageData();
   };
   std::string load_file(std::string file_path);

   class Texture2D
   {
   public:
      uint id;

      Texture2D();
      void bind();
      void unbind();
      void set_int(int param_name, int param_value);
      void set_source(int texture_color_format, int width, int height, int source_image_color_format, u_char *data);
      void gen_mipmap();
      static Texture2D FromImage(std::string path);
   };

}

template <typename T>
void Gamma::VertexElementBuffer::buffer_data(std::vector<T> &indexes)
{
   bind();
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes.size() * sizeof(T), indexes.data(), GL_STATIC_DRAW);
}

template <typename T>
void Gamma::VertexDataBuffer::buffer_data(std::vector<T> &vertices)
{
   bind();
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);
}


#endif