#pragma once

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <tiny_obj_loader.h>
#include <vector>

namespace my {


struct ObjDetails
{
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
};


ObjDetails LoadObj(const std::filesystem::path &file_path)
{
    tinyobj::attrib_t                attrib;
    std::vector<tinyobj::shape_t>    shapes;
    std::vector<tinyobj::material_t> materials;
    std::string                      warn, err;


    std::ifstream ifs(file_path);
    if (!ifs.is_open() || ifs.fail()) {
        throw std::runtime_error("file to open file");
    }
    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, &ifs);

    if (warn.size() > 0)
        std::cerr << warn << '\n';
    if (err.size() > 0)
        std::cerr << err << '\n';


    return {attrib, shapes, materials};
}
}; // namespace my
