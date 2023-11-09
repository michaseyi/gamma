#include "Geometry.h"

namespace Gamma::Geometry
{
   glm::vec3 **allocate_vertex_buffer(int rows, int columns)
   {
      glm::vec3 **rows_buffer = (glm::vec3 **)malloc(sizeof(glm::vec3 *) * rows);
      for (int i{0}; i < rows; i++)
         rows_buffer[i] = (glm::vec3 *)malloc(sizeof(glm::vec3) * columns);
      return rows_buffer;
   }

   void free_vertex_buffer(glm::vec3 **rows_buffer, int rows, int columns)
   {
      for (int i{0}; i < rows; i++)
      {
         free(rows_buffer[i]);
      }
      free(rows_buffer);
   }

   float *gen_trianges(glm::vec3 **vertices, int rows, int columns, uint *size, uint *vertice_count)
   {
      int offset = 0;
      int next_i;
      int next_j;
      *size = rows * columns * 2 * 3 * 6;
      *vertice_count = *size / 6;
      float *triangles = (float *)malloc(sizeof(float) * (*size));
      for (int i{0}; i < rows; i++)
      {
         for (int j{0}; j < columns; j++)
         {
            next_i = (i + 1) % rows;
            next_j = (j + 1) % columns;

            auto a = vertices[i][j];
            auto b = vertices[next_i][j];
            auto c = vertices[next_i][next_j];
            // Triangle 1 [v[i,j], v[next_i, j], v[next_i, next_j]]

            auto normal = glm::cross(-glm::normalize(b - a), glm::normalize(c - a));
            memcpy(triangles + offset, glm::value_ptr(a), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;

            memcpy(triangles + offset, glm::value_ptr(b), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;

            memcpy(triangles + offset, glm::value_ptr(c), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;

            b = vertices[i][next_j];

            normal = glm::cross(glm::normalize(b - a), glm::normalize(c - a));
            memcpy(triangles + offset, glm::value_ptr(a), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;

            memcpy(triangles + offset, glm::value_ptr(b), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;

            memcpy(triangles + offset, glm::value_ptr(c), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;
            // Triangle 2 [v[i, j], v[i, next_j], v[next_i, next_j]]
         }
      }
      return triangles;
   }
   float *gen_trianges_without_wrap(glm::vec3 **vertices, int rows, int columns, uint *size, uint *vertice_count)
   {
      int offset = 0;
      int next_i;
      int next_j;
      rows--;
      columns--;
      *size = rows * columns * 2 * 3 * 6;
      *vertice_count = *size / 6;
      float *triangles = (float *)malloc(sizeof(float) * (*size));
      for (int i{0}; i < rows; i++)
      {
         for (int j{0}; j < columns; j++)
         {
            next_i = (i + 1);
            next_j = (j + 1);

            auto a = vertices[i][j];

            auto b = vertices[next_i][j];
            auto c = vertices[next_i][next_j];
            // Triangle 1 [v[i,j], v[next_i, j], v[next_i, next_j]]

            auto normal = glm::cross(-glm::normalize(b - a), glm::normalize(c - a));

            memcpy(triangles + offset, glm::value_ptr(a), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;

            memcpy(triangles + offset, glm::value_ptr(b), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;

            memcpy(triangles + offset, glm::value_ptr(c), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;

            b = vertices[i][next_j];

            // normal = glm::cross(glm::normalize(b - a), -glm::normalize(c - a));
            memcpy(triangles + offset, glm::value_ptr(a), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;

            memcpy(triangles + offset, glm::value_ptr(b), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;

            memcpy(triangles + offset, glm::value_ptr(c), sizeof(float) * 3);
            offset += 3;
            memcpy(triangles + offset, glm::value_ptr(normal), sizeof(float) * 3);
            offset += 3;
            // Triangle 2 [v[i, j], v[i, next_j], v[next_i, next_j]]
         }
      }
      return triangles;
   }

   Cone::Cone(float radius, int mesh_detail_level, float height_offset)
   {
      int n = mesh_detail_level;
      float delta_theta = glm::two_pi<float>() / n;
      float theta = 0.0f;
      glm::vec3 **vertices = allocate_vertex_buffer(n, 3);

      float height = radius * 2;

      for (int i{0}; i < n; i++)
      {
         theta = delta_theta * i;
         vertices[i][0] = glm::vec3(0.0f, height_offset, 0.0f);
         vertices[i][1] = glm::vec3(
             glm::cos(theta) * radius,
             height_offset,
             glm::sin(theta) * radius);

         vertices[i][2] = glm::vec3(0.0f, height + height_offset, 0.0f);
      }
      triangles = gen_trianges(vertices, n, 3, &size, &vertex_count);
      free_vertex_buffer(vertices, n, 3);
   }

   void Cone::bind()
   {
      VertexDataBuffer buffer;
      buffer.buffer_data_from_pointer(triangles, size);
      buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
      buffer.set_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);
   }

   Cone::~Cone()
   {
      free(triangles);
   }

   Cylinder::Cylinder(float radius, int mesh_detail_level, float height)
   {
      int n = mesh_detail_level;
      float delta_theta = glm::two_pi<float>() / n;
      float theta = 0.0f;
      glm::vec3 **vertices = allocate_vertex_buffer(n, 4);
      for (int i{0}; i < n; i++)
      {
         theta = delta_theta * i;
         vertices[i][0] = glm::vec3(0.0f);
         vertices[i][1] = glm::vec3(
             glm::cos(theta) * radius,
             0.0f,
             glm::sin(theta) * radius);

         vertices[i][2] = glm::vec3(
             glm::cos(theta) * radius,
             height,
             glm::sin(theta) * radius);

         vertices[i][3] = glm::vec3(0.0f, height, 0.0f);
      }

      triangles = gen_trianges(vertices, n, 4, &size, &vertex_count);
      free_vertex_buffer(vertices, n, 4);
   }

   void Cylinder::bind()
   {
      VertexDataBuffer buffer;
      buffer.buffer_data_from_pointer(triangles, size);
      buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
      buffer.set_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);
   }

   Cylinder::~Cylinder()
   {
      free(triangles);
   }

   Arrow::Arrow(float radius, int mesh_detail_level, float height)
   {
      Cone cone{radius, mesh_detail_level, height};
      Cylinder cylinder{radius / 3, mesh_detail_level, height};
      size = cone.size + cylinder.size;
      vertex_count = cone.vertex_count + cylinder.vertex_count;
      triangles = (float *)malloc(sizeof(float) * size);
      memcpy(triangles, cone.triangles, sizeof(float) * cone.size);
      memcpy(triangles + cone.size, cylinder.triangles, sizeof(float) * cylinder.size);

      vao = VertexArrayObject{};
      vao.bind();
      VertexDataBuffer buffer;
      buffer.buffer_data_from_pointer(triangles, size);
      buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
      buffer.set_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);
      vao.unbind();
   }

   void Arrow::bind()
   {
      vao.bind();
   }

   void Arrow::draw()
   {
      vao.bind();
      glDrawArrays(GL_TRIANGLES, 0, vertex_count);
      vao.unbind();
   }

   Arrow::~Arrow()
   {
      free(triangles);
   }
   Sphere::Sphere(float radius, int mesh_detail_level)
   {
      int n = mesh_detail_level, m = n / 2;
      float x, y, z;
      float delta_theta = glm::two_pi<float>() / n, delta_phi = glm::pi<float>() / m, theta = 0.0f, phi = 0.0f;
      glm::vec3 **vertices = allocate_vertex_buffer(n, m + 1);
      for (int i{0}; i < n; i++)
      {
         for (int j{0}; j < m + 1; j++)
         {
            theta = i * delta_theta;
            phi = j * delta_phi;
            x = radius * glm::sin(phi) * glm::cos(theta);
            y = radius * glm::sin(phi) * glm::sin(theta);
            z = radius * glm::cos(phi);
            vertices[i][j] = glm::vec3(x, y, z);
         }
      }
      m += 1;
      triangles = gen_trianges(vertices, n, m, &size, &vertex_count);
      free_vertex_buffer(vertices, n, m);
      vao = Gamma::VertexArrayObject{};
      vao.bind();
      VertexDataBuffer buffer;
      buffer.buffer_data_from_pointer(triangles, size);
      buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
      buffer.set_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);
      vao.unbind();
   }

   void Sphere::bind()
   {
      vao.bind();
   }
   void Sphere::draw()
   {
      vao.bind();
      glDrawArrays(GL_TRIANGLES, 0, vertex_count);
      vao.unbind();
   }
   Sphere::~Sphere()
   {
      free(triangles);
   }

   Donut::Donut(float radius, int mesh_detail_level)
   {
      int n = mesh_detail_level, m = n;
      float x, y, z;
      float delta_theta = glm::two_pi<float>() / n, delta_phi = glm::two_pi<float>() / m, theta = 0.0f, phi = 0.0f;
      glm::vec3 **vertices = allocate_vertex_buffer(n, m);
      for (int i{0}; i < n; i++)
      {
         for (int j{0}; j < m; j++)
         {
            auto c_x = radius * 3 * glm::cos(theta);
            auto c_z = radius * 3 * glm::sin(theta);
            auto c_y = 0;
            glm::vec3 center(c_x, c_y, c_z);
            theta = i * delta_theta;
            phi = j * delta_phi;
            center = center + (glm::normalize(center) * glm::cos(phi));
            x = center.x;
            z = center.z;
            y = radius * glm::sin(phi);
            vertices[i][j] = glm::vec3(x, y, z);
         }
      }
      triangles = gen_trianges(vertices, n, m, &size, &vertex_count);
      free_vertex_buffer(vertices, n, m);

      vao = VertexArrayObject{};
      vao.bind();
      VertexDataBuffer buffer;
      buffer.buffer_data_from_pointer(triangles, size);
      buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
      buffer.set_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);
      vao.unbind();
   }

   void Donut::bind()
   {
      vao.bind();
   }
   void Donut::draw()
   {
      vao.bind();
      glDrawArrays(GL_TRIANGLES, 0, vertex_count);
      vao.unbind();
   }
   Donut::~Donut()
   {
      free(triangles);
   }

   Landscape::Landscape(std::string height_map_path, int mesh_detail_level)
   {
      ImageData height_map(height_map_path);
      auto width = height_map.width;
      auto height = height_map.height;
      int n = mesh_detail_level, m = n;
      n = height_map.height;
      m = height_map.width;
      float x, y, z;
      float delta_x = 30.0f / n, delta_z = 30.0f / m, c_x = 0.0f, c_z = 0.0f;
      glm::vec3 **vertices = allocate_vertex_buffer(n, m);
      for (int i{0}; i < n; i++)
      {
         for (int j{0}; j < m; j++)
         {
            x = i * delta_x;
            z = j * delta_z;
            y = 0;

            for (int k{0}; k < height_map.n_channels; k++)
            {
               y += height_map.data[i * width * height_map.n_channels + height_map.n_channels * (j + k)] / 255.0f;
            }
            y /= height_map.n_channels;
            y *= 2.0f;
            y = glm::sin(x * x + z * z);
            vertices[i][j] = glm::vec3(x, y, z);
         }
      }
      triangles = gen_trianges_without_wrap(vertices, n, m, &size, &vertex_count);
      free_vertex_buffer(vertices, n, m);
      vao.bind();
      VertexDataBuffer buffer;
      buffer.buffer_data_from_pointer(triangles, size);
      buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
      buffer.set_attrib_pointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);
      vao.unbind();
   }

   void Landscape::bind()
   {
      vao.bind();
   }
   void Landscape::draw()
   {
      vao.bind();
      glDrawArrays(GL_TRIANGLES, 0, vertex_count);
      vao.unbind();
   }

   Landscape::~Landscape()
   {
      free(triangles);
   }

   Mat::Mat()
   {
      for (int i{-10}; i <= 10; i++)
      {
         lines.emplace_back(glm::vec3{-10.0f, 0.0f, i});
         lines.emplace_back(glm::vec3{10.0f, 0.0f, i});
      }
      for (int i{-10}; i <= 10; i++)
      {
         lines.emplace_back(glm::vec3{i, 0.0f, -10.0f});
         lines.emplace_back(glm::vec3{i, 0.0f, 10.0f});
      }

      vao.bind();
      auto buffer = VertexDataBuffer{};
      buffer.buffer_data_from_pointer((float *)lines.data(), lines.size() * 3);
      buffer.set_attrib_pointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
      vao.unbind();
   }

   void Mat::bind()
   {
      vao.bind();
   }

   void Mat::draw()
   {
      vao.bind();
      glDrawArrays(GL_LINES, 0, lines.size());
      vao.unbind();
   }

}