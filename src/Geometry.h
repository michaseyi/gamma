#ifndef __GEOMETRY__
#define __GEOMETRY__
#include "Gamma.h"

namespace Gamma::Geometry
{
   class Cube
   {
   };

   class Landscape
   {
      public:
         Landscape(std::string height_map_path, int mesh_detail_level);
         uint size;
         uint vertex_count;
         VertexArrayObject vao;
         float *triangles;
         void bind();
         void draw();
       ~Landscape();
   };

   class Sphere
   {
   public:
      float radius;
      uint size;
      uint vertex_count;
      float *triangles;
      VertexArrayObject vao;
      void bind();
      void draw();
      Sphere(float radius, int mesh_detail_level);
      ~Sphere();
   };

   class Donut
   {
   public:
      float radius;
      uint size;
      VertexArrayObject vao;
      uint vertex_count;
      float *triangles;
      void bind();
      void draw();
      Donut(float radius, int mesh_detail_level);
      ~Donut();
   };
   class Cone
   {
   public:
      uint size;
      uint vertex_count;
      float radius;
      Cone(float radius, int mesh_detail_level, float height_offset);
      float *triangles;
      ~Cone();
      void bind();
   };

   class Arrow
   {
   public:
      uint size;
      uint vertex_count;
      VertexArrayObject vao;
      float radius_cone;
      float radius_cylinder;
      float height;
      float *triangles;
      void draw();
      void bind();
      Arrow(float radius, int mesh_detail_level, float height);
      ~Arrow();
   };

   class Mat {
      public: 
      std::vector<glm::vec3> lines;
      void draw();
      void bind();
      VertexArrayObject vao;
      Mat();
      // ~Mat();
   };
   class Cylinder
   {
   public:
      uint size;
      uint vertex_count;
      float radius;
      float *triangles;
      Cylinder(float radius, int mesh_detail_level, float height);
      ~Cylinder();
      void bind();
   };

}

#endif