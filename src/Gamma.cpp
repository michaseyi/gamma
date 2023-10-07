#include "Gamma.h"
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image/stb_image.h"

Gamma::Engine *Gamma::Engine::New(std::string app_name, float aspect_ratio, uint width)
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


   window = glfwCreateWindow(width, height, app_name.c_str(), nullptr, nullptr);
   if (window == nullptr)
   {
      std::cerr << "Error initializing Gamma Engine: Failed to create window" << std::endl;
      glfwTerminate();
      exit(2);
   }
   engine->current_window = window;
   glfwMakeContextCurrent(engine->current_window);
   //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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

void Gamma::Engine::clear_buffer()
{
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Gamma::Engine *Gamma::Engine::set_callback(CallbackFunc callback)
{
   callbacks.push_back(callback);
   return this;
}

void Gamma::Engine::run_callbacks()
{
   for (auto callback : callbacks)
   {
      callback(current_window);
   }
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

void Gamma::ShaderProgram::set_mat4(std::string name, glm::mat4 &value)
{
   glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
}
void Gamma::ShaderProgram::set_int(std::string name, int value)
{
   glUniform1i(get_uniform_location(name), value);
}

Gamma::VertexDataBuffer::VertexDataBuffer()
{
   glGenBuffers(1, &id);
}

void Gamma::VertexDataBuffer::unbind()
{
   glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void Gamma::VertexArrayObject::unbind()
{
   glBindVertexArray(0);
}

Gamma::VertexElementBuffer::VertexElementBuffer()
{
   glGenBuffers(1, &id);
   bind();
}

void Gamma::VertexElementBuffer::unbind()
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void Gamma::VertexElementBuffer::bind()
{
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

Gamma::ImageData::ImageData(std::string path)
{
   this->data = stbi_load(path.c_str(), &width, &height, &n_channels, 0);
   if (!this->data)
   {
      std::cerr << "Gamma Engine error: Error loading image data: " << path << std::endl;
      exit(2);
   }
}

Gamma::ImageData::~ImageData()
{
   if (this->data)
      stbi_image_free(this->data);
}

Gamma::Texture2D::Texture2D()
{
   glGenTextures(1, &id);
   this->bind();
}

void Gamma::Texture2D::bind()
{
   glBindTexture(GL_TEXTURE_2D, id);
}

void Gamma::Texture2D::gen_mipmap()
{
   glGenerateMipmap(id);
   bind();
}

void Gamma::Texture2D::set_source(int texture_color_format, int width, int height, int source_image_color_format, u_char *data)
{
   bind();
   glTexImage2D(GL_TEXTURE_2D, 0, texture_color_format, width, height, 0, source_image_color_format, GL_UNSIGNED_BYTE, data);
}

void Gamma::Texture2D::unbind()
{
   glBindTexture(GL_TEXTURE_2D, 0);
}

void Gamma::Texture2D::set_int(int param_name, int param_value)
{
   glTexParameteri(GL_TEXTURE_2D, param_name, param_value);
}

Gamma::Texture2D Gamma::Texture2D::FromImage(std::string path)
{
   ImageData data(path);

   Texture2D texture;
   texture.set_source(GL_RGB, data.width, data.height, GL_RGB, data.data);
   texture.gen_mipmap();

   texture.set_int(GL_TEXTURE_WRAP_S, GL_REPEAT);
   texture.set_int(GL_TEXTURE_WRAP_T, GL_REPEAT);
   texture.set_int(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   texture.set_int(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

   return texture;
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