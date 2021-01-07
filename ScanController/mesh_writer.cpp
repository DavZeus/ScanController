#include "mesh_writer.h"

#include <CGAL/IO/STL_writer.h>
#include <fstream>

auto mesh_writer::write(std::string path, const surface_mesh &mesh) {
  std::ofstream file(path);
  CGAL::write_STL(mesh, file);
}
