#pragma once


#include "./game_object.h"
#include <string>


const glm::vec2 SIZE(60.f, 20.f);
const glm::vec2 VELOCITY(0.f, 150.f);

class PowerUp : public GameObject
{

  public:
    PowerUp(std::string type, glm::vec3 color, float duration, glm::vec2 pos, Texture2D texture)
        : GameObject(pos, SIZE, texture, color, VELOCITY),
          Type(type), Duration(duration), bActivated(false) {}

  public:
    std::string Type;
    float       Duration;
    bool        bActivated;
};