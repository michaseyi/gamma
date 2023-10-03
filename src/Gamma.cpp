
#include "Gamma.h"
#include <fstream>
#include <sstream>

Gamma::Engine *Gamma::Engine::New(std::string app_name, uint width, uint height)
{
   auto engine = new Engine();
   engine->height = height;
   engine->width = width;
   engine->app_name = app_name;
   GLFWwindow *window;

   glfwInit();
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   window = glfwCreateWindow(width, height, app_name.c_str(), nullptr, nullptr);
   if (window == nullptr)
   {
      std::cerr << "Error initializing Gamma Engine: Failed to create window" << std::endl;
      glfwTerminate();
      exit(2);
   }
   engine->current_window = window;
   glfwMakeContextCurrent(engine->current_window);

   // Initializing glad
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
   {
      std::cerr << "Error initializing Gamma Engine: Failed to initialize glad" << std::endl;
      glfwTerminate();
      exit(2);
   }

   glViewport(0, 0, width, height);
   glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height)
                                  {
      if (width > height)
         glViewport((width - height) / 2, 0, height, height);
      else if (height > width)
         glViewport(0, (height - width) / 2,width, width ); 
      else glViewport(0, 0, width, height); });
   return engine;
}

void Gamma::Engine::clear_buffer()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);
}

Gamma::Engine *Gamma::Engine::swap_buffer()
{
   glfwSwapBuffers(current_window);
   return this;
}
Gamma::Engine *Gamma::Engine::poll_events()
{
   glfwPollEvents();
   return this;
}

std::string Gamma::load_file(std::string file_path)
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

Gamma::ShaderProgram::ShaderProgram(std::string vert_path, std::string frag_path)
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

void Gamma::ShaderProgram::use()
{
   glUseProgram(id);
}

int Gamma::ShaderProgram::get_uniform_location(std::string uniform_name)
{
   return glGetUniformLocation(id, uniform_name.c_str());
}

void Gamma::ShaderProgram::set_bool(std::string name, bool value)
{
   glUniform1i(get_uniform_location(name), value);
}

void Gamma::ShaderProgram::set_float(std::string name, float value)
{
   glUniform1f(get_uniform_location(name), value);
}

void Gamma::ShaderProgram::set_int(std::string name, int value)
{
   glUniform1i(get_uniform_location(name), value);
}

Gamma::VertexDataBuffer::VertexDataBuffer()
{
   glGenBuffers(1, &id);
}

void Gamma::VertexDataBuffer::bind()
{
   glBindBuffer(GL_ARRAY_BUFFER, id);
}

void Gamma::VertexDataBuffer::set_attrib_pointer(uint location, uint length, uint data_type, bool normalize, uint stride, uint64_t offset)
{
   bind();
   glVertexAttribPointer(location, length, data_type, normalize, stride, (void *)offset);
   enable_attrib_pointer(location);
}

void Gamma::VertexDataBuffer::enable_attrib_pointer(uint location)
{
   glEnableVertexAttribArray(location);
}

Gamma::VertexArrayObject::VertexArrayObject()
{
   glGenVertexArrays(1, &id);
   bind();
}

void Gamma::VertexArrayObject::bind()
{
   glBindVertexArray(id);
}

Gamma::VertexElementBuffer::VertexElementBuffer()
{
   glGenBuffers(1, &id);
   bind();
}

void Gamma::VertexElementBuffer::bind()
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}
