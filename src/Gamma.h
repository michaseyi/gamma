#ifndef __GAMMA__
#define __GAMMA__
#include "../lib/glad/glad.h"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <vector>

namespace Gamma
{

   class Engine
   {
      uint width;
      uint height;
      std::string app_name;

   public:
      GLFWwindow *current_window;
      static Engine *New(std::string app_name, uint width, uint height);
      void clear_buffer();
      Engine *swap_buffer();
      Engine *poll_events();
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
   };

   class VertexDataBuffer
   {
   public:
      uint id;

      VertexDataBuffer();
      void bind();
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
      template <typename T>
      void buffer_data(std::vector<T> &indexes);
   };

   class VertexArrayObject
   {
   public:
      uint id;
      VertexArrayObject();
      void bind();
   };

   class Texture
   {

      void shout()
      {
         float a[] = {0.0, 0.0, 0.0, 0.0};

         // wrapping
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

         // sampling
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

         // setting a parameter
         glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, a);
      }
   };

   std::string load_file(std::string file_path);

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