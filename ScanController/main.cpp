#include "dimension_converter.h"
#include "mesh_writer.h"
#include "model_constructor.h"
#include "sc_options.h"
#include "scan_handler.h"

#include <fmt/core.h>
#include <fstream>
#include <sstream>

int main(int argc, char *argv[]) {
  try {
    /*sc_options options{};
    options.parse(argc, argv);
    const auto com =
        "COM" +
        std::to_string(options.get_value(sc_options::com_switch).as<short>());

    scan_handler scanner(com);
    auto points = scanner.start();
    std::ofstream temp("points.txt");
    for (const auto &vert : points) {
            for (const auto &d_point : vert) {
        temp << '\n' << d_point.first << ' ' << d_point.second << std::flush; 
      }
      temp << "\n^" << std::flush;
    }
    temp.close();*/
    std::vector < std::vector<std::pair<float, float>>> points;
    std::ifstream pf("points.txt");
    while (!pf.eof()) {
      static std::vector<std::pair<float, float>> vert;
      static std::string buffer;
      std::getline(pf, buffer, '\n');
      if (buffer == "^") {
        points.emplace_back(vert);
        vert.clear();
      }
      static float x;
      static float z;
      std::stringstream s(buffer);
      s >> x >> z;
      vert.emplace_back(x, z);
    }
    //pf.close();
    const dimension_converter converter(396.F, 1530.F);
    auto p = converter.convert(std::move(points));
    const model_constructor constructor(
        model_constructor::methods::advancing_front);
    auto mesh = constructor.make_mesh(p);
    mesh_writer::write("model.stl", mesh);
  } catch (const std::exception &ex) {
    fmt::print(stderr, "\n{}\n", ex.what());
  }
}
