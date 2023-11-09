#include "Gamma.h"
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image/stb_image.h"

namespace Gamma
{
   Engine *Engine::New(std::string app_name, float aspect_ratio, uint width)
   {
      auto engine = new Engine();

      int height = (width / aspect_ratio);
      engine->height = height;
      engine->width = width;
      engine->app_name = app_name;
      GLFWwindow *window;

      glfwInit();
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      // glfwWindowHint(GLFW_DECORATED, GL_FALSE);
      // glfwWindowHint(0x0002000A, 1);

      window = glfwCreateWindow(width, height, app_name.c_str(), nullptr, nullptr);
      if (window == nullptr)
      {
         std::cerr << "Error initializing Gamma Engine: Failed to create window" << std::endl;
         glfwTerminate();
         exit(2);
      }
      engine->current_window = window;
      glfwMakeContextCurrent(engine->current_window);
      // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

      // Initializing glad
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      {
         std::cerr << "Error initializing Gamma Engine: Failed to initialize glad" << std::endl;
         glfwTerminate();
         exit(2);
      }

      glEnable(GL_DEPTH_TEST);
      glViewport(0, 0, width, height);
      return engine;
   }

   void Engine::clear_buffer()
   {
      glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   }

   Engine *Engine::set_callback(CallbackFunc callback)
   {
      callbacks.push_back(callback);
      return this;
   }

   void Engine::run_callbacks()
   {
      for (auto callback : callbacks)
      {
         callback(current_window);
      }
   }

   Engine *Engine::swap_buffer()
   {
      glfwSwapBuffers(current_window);
      return this;
   }
   Engine *Engine::poll_events()
   {
      glfwPollEvents();
      return this;
   }

   std::string load_file(std::string file_path)
   {
      std::ifstream file(file_path);
      std::stringstream ss;

      if (file.is_open())
      {
         ss << file.rdbuf();
         file.close();
         return ss.str();
      }
      std::cerr << "Gamma Engine error: Failed to readfile: " << file_path << std::endl;
      exit(2);
   }

   ShaderProgram::ShaderProgram(std::string vert_path, std::string frag_path)
   {
      uint vert_shader = glCreateShader(GL_VERTEX_SHADER);
      auto vert_shader_source = load_file(vert_path);
      auto vert_shader_cstr = vert_shader_source.c_str();
      glShaderSource(vert_shader, 1, &vert_shader_cstr, nullptr);
      glCompileShader(vert_shader);

      int success;
      char info_log[512];
      glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
         glGetShaderInfoLog(vert_shader, 512, nullptr, info_log);
         std::cerr << "Gamma Engine error: Error compiling vertex shader: " << info_log << std::endl;
         exit(2);
      }

      uint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
      auto frag_shader_source = load_file(frag_path);
      auto frag_shader_cstr = frag_shader_source.c_str();

      glShaderSource(frag_shader, 1, &frag_shader_cstr, nullptr);
      glCompileShader(frag_shader);
      glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
         glGetShaderInfoLog(frag_shader, 512, nullptr, info_log);
         std::cerr << "Gamma Engine error: Error compiling fragment shader: " << info_log << std::endl;
         exit(2);
      }

      uint shader_program = glCreateProgram();
      this->id = shader_program;

      glAttachShader(id, vert_shader);
      glAttachShader(id, frag_shader);
      glLinkProgram(id);

      glGetProgramiv(id, GL_LINK_STATUS, &success);
      if (!success)
      {
         glGetProgramInfoLog(id, 512, nullptr, info_log);
         std::cerr << "Gamma Engine error: Error linking shader program: " << info_log << std::endl;
         exit(2);
      }
      glDeleteShader(vert_shader);
      glDeleteShader(frag_shader);
   }

   void ShaderProgram::use()
   {
      glUseProgram(id);
   }

   int ShaderProgram::get_uniform_location(std::string uniform_name)
   {
      return glGetUniformLocation(id, uniform_name.c_str());
   }

   void ShaderProgram::set_bool(std::string name, bool value)
   {
      glUniform1i(get_uniform_location(name), value);
   }

   void ShaderProgram::set_vec3(std::string name, glm::vec3 &value)
   {
      glUniform3fv(get_uniform_location(name), 1, glm::value_ptr(value));
   }

   void ShaderProgram::set_float(std::string name, float value)
   {
      glUniform1f(get_uniform_location(name), value);
   }

   void ShaderProgram::set_mat4(std::string name, glm::mat4 &value)
   {
      glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
   }
   void ShaderProgram::set_int(std::string name, int value)
   {
      glUniform1i(get_uniform_location(name), value);
   }

   VertexDataBuffer::VertexDataBuffer()
   {
      glGenBuffers(1, &id);
   }

   void VertexDataBuffer::unbind()
   {
      glBindBuffer(GL_ARRAY_BUFFER, 0);
   }
   void VertexDataBuffer::bind()
   {
      glBindBuffer(GL_ARRAY_BUFFER, id);
   }

   void VertexDataBuffer::set_attrib_pointer(uint location, uint length, uint data_type, bool normalize, uint stride, uint64_t offset)
   {
      bind();
      glVertexAttribPointer(location, length, data_type, normalize, stride, (void *)offset);
      enable_attrib_pointer(location);
   }

   void VertexDataBuffer::enable_attrib_pointer(uint location)
   {
      glEnableVertexAttribArray(location);
   }

   VertexArrayObject::VertexArrayObject()
   {
      glGenVertexArrays(1, &id);
      bind();
   }

   void VertexArrayObject::bind()
   {
      glBindVertexArray(id);
   }

   void VertexArrayObject::unbind()
   {
      glBindVertexArray(0);
   }

   VertexElementBuffer::VertexElementBuffer()
   {
      glGenBuffers(1, &id);
      bind();
   }

   void VertexElementBuffer::unbind()
   {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   }
   void VertexElementBuffer::bind()
   {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
   }

   ImageData::ImageData(std::string path)
   {
      this->data = stbi_load(path.c_str(), &width, &height, &n_channels, 0);
      if (!this->data)
      {
         std::cerr << "Gamma Engine error: Error loading image data: " << path << std::endl;
         exit(2);
      }
   }

   ImageData::~ImageData()
   {
      if (this->data)
         stbi_image_free(this->data);
   }

   Texture2D::Texture2D(int texture_unit)
   {
      this->texture_unit = texture_unit;
      glGenTextures(1, &id);
      this->bind();
   }

   void Texture2D::bind()
   {
      glActiveTexture(texture_unit);
      glBindTexture(GL_TEXTURE_2D, id);
   }

   void Texture2D::gen_mipmap()
   {
      bind();
      glGenerateMipmap(id);
   }

   void Texture2D::set_source(int texture_color_format, int width, int height, int source_image_color_format, u_char *data)
   {
      bind();
      glTexImage2D(GL_TEXTURE_2D, 0, texture_color_format, width, height, 0, source_image_color_format, GL_UNSIGNED_BYTE, data);
   }

   void Texture2D::unbind()
   {
      glBindTexture(GL_TEXTURE_2D, 0);
   }

   void Texture2D::set_int(int param_name, int param_value)
   {
      glTexParameteri(GL_TEXTURE_2D, param_name, param_value);
   }

   Texture2D Texture2D::FromImage(std::string path, int texture_unit)
   {
      ImageData data(path);

      Texture2D texture(texture_unit);
      texture.set_source(GL_RGB, data.width, data.height, GL_RGB, data.data);
      texture.gen_mipmap();

      texture.set_int(GL_TEXTURE_WRAP_S, GL_REPEAT);
      texture.set_int(GL_TEXTURE_WRAP_T, GL_REPEAT);
      texture.set_int(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      texture.set_int(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      return texture;
   }
   Texture2D Texture2D::FromImagePNG(std::string path, int texture_unit)
   {
      ImageData data(path);

      Texture2D texture(texture_unit);
      texture.set_source(GL_RGB, data.width, data.height, GL_RGBA, data.data);
      texture.gen_mipmap();

      texture.set_int(GL_TEXTURE_WRAP_S, GL_REPEAT);
      texture.set_int(GL_TEXTURE_WRAP_T, GL_REPEAT);
      texture.set_int(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      texture.set_int(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      return texture;
   }

   void VertexDataBuffer::buffer_data_from_pointer(float *source, u_int size)
   {
      bind();
      glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), source, GL_STATIC_DRAW);
   }
}
std::ostream &operator<<(std::ostream &output, glm::vec4 &vec)
{
   output << "[" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << "]" << std::endl;
   return output;
}

std::ostream &operator<<(std::ostream &output, glm::vec3 &vec)
{
   output << "[" << vec.x << " " << vec.y << " " << vec.z << "]" << std::endl;
   return output;
}

std::ostream &operator<<(std::ostream &output, glm::mat4 &mat)
{
   for (uint i = 0; i < 4; i++)
   {
      output << mat[i];
   }
   return output;
}