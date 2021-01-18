#include "file_writer.h"

#include <CGAL/IO/STL_writer.h>
#include <fstream>

auto sc::file_writer::write_mesh(std::string path, const surface_mesh &mesh)
    -> void {
  std::ofstream file(path);
  CGAL::write_STL(mesh, file);
  // Write content to file
  file.flush();
}
