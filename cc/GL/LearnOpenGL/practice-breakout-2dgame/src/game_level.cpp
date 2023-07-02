#include "game_level.h"
#include "render/game_object.h"
#include "resource_manager/resource_manager.h"
#include <cmath>
#include <fstream>
#include <glm/fwd.hpp>
#include <math.h>
#include <sstream>
#include <string>


void GameLevel::Load(const char *file, uint level_width, uint level_height)
{
    using std::vector;

    std::ifstream ifs(file);
    if (ifs.fail() || !ifs.is_open()) {
        LOG_ERROR("load level file {} failed", file);
        return;
    }

    this->Bricks.clear();

    uint        tile_code;
    std::string line;

    vector<vector<uint>> tile_data;

    while (std::getline(ifs, line))
    {
        std::vector<uint>  row;
        std::istringstream ss(line);

        // from string(raw hex) to unsigned int
        // and when reach a space will turn to next loop
        while (ss >> tile_code) {
            row.push_back(tile_code);
        }

        tile_data.push_back(row);
    }

    if (tile_data.size() > 0) {
        init(tile_data, level_width, level_height);
    }
}

void GameLevel::Draw(SpriteRender &render)
{
    for (auto &brick : Bricks) {
        brick.Draw(render);
    }
}

bool GameLevel::IsCompleted() { return false; }

void GameLevel::init(std::vector<std::vector<uint>> tile_data, uint level_width, uint level_height)
{
    using glm::vec2, glm::vec3;

    int  posX = 0, posY = 0;
    auto sizeX = level_width / (float)tile_data.size();
    auto sizeY = level_height / (float)tile_data[0].size();


    for (auto &row : tile_data)
    {
        for (auto ui : row)
        {
            vec3       color;
            uint       value = ui;
            GameObject obj;
            if (value != 0) {
                if (value == 1) {
                    color = vec3(0.9, 0.9, 0.9);
                }
                else {
                    color = vec3(sin(value), cos(value), tan(value));
                }

                obj = GameObject(vec2(posX, posY),
                                 vec2(sizeX, sizeY),
                                 ResourceManager::GetTexture("arch"),
                                 color,
                                 vec2(0, 0));
            }

            Bricks.push_back(obj);

            posX += sizeX;
        }
        posY += sizeY;
    }
}
