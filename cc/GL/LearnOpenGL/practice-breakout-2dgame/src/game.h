#pragma once

#include "game_level.h"
#include "glm/fwd.hpp"
#include "obj/ball_object.h"
#include "render/sprite_render.h"
#include <string>
#include <unordered_map>
enum class GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
};

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};
;


inline const glm::vec2 PLAYER_SIZE{100.f, 20.f};
inline const float     PLAYER_VELOCITY{500.f};
inline const float     BALL_RADIUS = 12.5f;
inline const glm::vec2 INITIAL_BALL_VELOCITY(100.f, -350.f);

class Game
{
    using uint = unsigned int;

  public:
    Game(uint w, uint h);
    ~Game();
    void Init();

  public:
    void ProcessInput(float dt);
    void DoCollisions();
    void Update(float dt);
    void Render();
    void ResetLevel();
    void ResetPlayer();

  private:

    void debugDraw();

  public:

    GameState m_State;

    std::unordered_map<std::string, SpriteRender> SpriteRenders;

    bool         m_keys[1024];
    unsigned int m_Width, m_Height;

    std::shared_ptr<GameObject> m_Player;
    std::shared_ptr<BallObject> m_Ball;
    std::vector<GameLevel>      m_Levels;
    size_t                      m_LevelIndex;
};
