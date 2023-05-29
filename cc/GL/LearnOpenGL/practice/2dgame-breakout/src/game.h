#pragma once

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

    bool         m_keys[1024];
    unsigned int m_Width, m_Height;
};
