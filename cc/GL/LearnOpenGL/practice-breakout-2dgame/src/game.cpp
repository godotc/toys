#include "game.h"
#include "render/game_object.h"
#include "render/sprite_render.h"

#include "resource_manager/resource_manager.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>


static auto shader = "sprite";

Game::Game(unsigned int width, unsigned int height) : m_State(GameState::GAME_ACTIVE), m_keys(), m_Width(width), m_Height(height) {}

Game::~Game() {}

void Game::Init()
{
    LOG("W: {} | H: {}", m_Width, m_Height);
    ResourceManager::LoadShader("../res/shaders/a.vert", "../res/shaders/a.frag", nullptr, shader);


    // view projection to resolute the [-1,1]
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->m_Width),
                                      static_cast<float>(this->m_Height), 0.0f,
                                      1.0f, -1.0f);
    ResourceManager::GetShader(shader).SetMatrix4("projection", projection);

    // reset image
    ResourceManager::GetShader(shader).Use().SetInteger("image", 0);
    Sprites[shader] = SpriteRender(ResourceManager::GetShader(shader));

    ResourceManager::LoadTexture("../res/textures/arch.png", true, "arch");

    // m_Level.Load("../res/levels/default_map", m_Width, m_Height);
}

void Game::Update(float dt)
{
}

void Game::ProcessInput(float dt)
{
}

void Game::Render()
{
    Sprites[shader].DrawSprite(ResourceManager::GetTexture("arch"),
                               glm::vec2(200.f, 200.f),
                               glm::vec2(200.f, 300.f),
                               45.f,
                               glm::vec3(1.f, 1.f, 0.f));

    // m_Level.Draw(Sprites[shader]);

    // it will draw in the sprite
    glPushMatrix();
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(0, 800);
    glVertex2f(800, 0);
    glVertex2f(0, -0.5);
    glVertex2f(-0.5, 0.5);
    glEnd();
    glPopMatrix();
}
