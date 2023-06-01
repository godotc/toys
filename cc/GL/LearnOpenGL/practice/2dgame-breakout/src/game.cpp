#include "game.h"
#include "log.h"
#include "resource_manager.h"
#include "sprite_render.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>

auto shader  = "sprite";
auto texture = "face";

Game::Game(unsigned int width, unsigned int height)
    : m_State(GameState::GAME_ACTIVE), m_keys(), m_Width(width), m_Height(height)
{
}

Game::~Game()
{
}

void Game::Init()
{
    ResourceManager::LoadShader("../res/shaders/a.vert",
                                "../res/shaders/a.frag",
                                nullptr, shader);
    LOG("W: {} | H: {}", m_Width, m_Height);


	// view projection to resolute the [-1,1]
    glm::mat4 projection = glm::ortho(
        0.0f, static_cast<float>(this->m_Width),
        static_cast<float>(this->m_Height), 0.0f,
        -1.0f, 1.0f);

    ResourceManager::GetShader(shader).SetMatrix4("projection", projection);

	// reset image
    ResourceManager::GetShader(shader).Use().SetInteger("image", 0);

    Sprites[shader] = SpriteRender(ResourceManager::GetShader(shader));

    ResourceManager::LoadTexture("../res/textures/arch.png", true, texture);
}

void Game::Update(float dt)
{
}

void Game::ProcessInput(float dt)
{
}

void Game::Render()
{
    Sprites[shader].DrawSprite(ResourceManager::GetTexture(texture),
                               glm::vec2(200.f, 200.f),
                               glm::vec2(200.f, 300.f),
                               45.f,
                               glm::vec3(0.f, 1.f, 0.f));

    // it will draw in the sprite
    /*
        glPushMatrix();
        glColor3f(1, 1, 1);
        glBegin(GL_LINES);
        glVertex2f(-1, -1);
        glVertex2f(1, 1);
        glVertex2f(-0.5, -0.5);
        glVertex2f(-0.5, 0.5);
        glEnd();
        glPopMatrix();
    */
}
