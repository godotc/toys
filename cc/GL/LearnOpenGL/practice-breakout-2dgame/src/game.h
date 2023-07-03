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

  private:
    void debugDraw();

  public:
    GameState m_State;

    std::unordered_map<std::string, SpriteRender> SpriteRenders;

    bool         m_keys[1024];
    unsigned int m_Width, m_Height;

    std::vector<GameLevel> m_Levels;
    size_t                 m_LevelIndex;
};
