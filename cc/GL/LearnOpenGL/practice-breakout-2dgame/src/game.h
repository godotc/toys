#pragma once

#include "glm/fwd.hpp"
#include "level/game_level.h"
#include "obj/ball_object.h"
#include "obj/power_up.h"
#include "particle/particle_generator.h"
#include "render/sprite_render.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>



class PostProcessor;


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

  public:
    void ResetLevel();
    void ResetPlayer();
    void SpawPowerUps(GameObject &block);
    void UpdatePowers(float dt);
    void ActivatePowerups(PowerUp &power_up);

  private:

    void debugDraw();

  public:

    GameState m_State;

    bool         m_keys[1024];
    unsigned int m_Width, m_Height;

    SpriteRender                       m_SpriteRnder;
    std::shared_ptr<GameObject>        m_Player;
    std::shared_ptr<BallObject>        m_Ball;
    std::shared_ptr<ParticleGenerator> m_Particles;
    std::shared_ptr<PostProcessor>     m_Effects;

    std::vector<GameLevel> m_Levels;
    size_t                 m_LevelIndex;

    std::vector<PowerUp> m_PowerUps;

  public:
    float ShakeTime = 0.f;
};
