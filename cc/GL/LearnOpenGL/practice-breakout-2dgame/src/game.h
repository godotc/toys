#pragma once

#include "game_level.h"
#include "render/sprite_render.h"
#include <string>
#include <unordered_map>
enum class GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

class Game
{
    using uint = unsigned int;

  public:
    Game(uint w, uint h);
    ~Game();
    void Init();

  public:
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();

  public:
    GameState m_State;

    std::unordered_map<std::string, SpriteRender> Sprites;

    bool         m_keys[1024];
    unsigned int m_Width, m_Height;
    GameLevel    m_Level;
};
