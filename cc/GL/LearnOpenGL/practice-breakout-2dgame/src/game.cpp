#include "game.h"
#include "render/sprite_render.h"
#include <gl_macros.h>

#include "resource_manager/resource_manager.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>
#include <vcruntime.h>
#include <vector>

#include <fmt/format.h>
#include <log.h>



static auto shader = "sprite";

Game::Game(unsigned int width, unsigned int height) : m_State(GameState::GAME_ACTIVE), m_keys(), m_Width(width), m_Height(height) {}

Game::~Game() {}

void Game::Init()
{
    LOG("W: {} | H: {}", m_Width, m_Height);

    ResourceManager::LoadShader("../res/shaders/a.vert", "../res/shaders/a.frag", nullptr, shader);

    // view projection to resolute the [-1,1]
    glm::mat4 projection = glm::ortho(0.0f, (float)this->m_Width,
                                      (float)this->m_Height, 0.0f,
                                      1.0f, -1.0f);

    // NOTICE: Must use this program first
    ResourceManager::GetShader(shader).Use();
    ResourceManager::GetShader(shader).SetMatrix4("projection", projection);
    // reset image
    ResourceManager::GetShader(shader).SetInteger("image", 0);
    ResourceManager::GetShader(shader).SetInteger("hasTexture", 1);

    Sprites[shader] = SpriteRender(ResourceManager::GetShader(shader));

    // load texture
    ResourceManager::LoadTexture("../res/textures/arch.png", true, "arch");
    ResourceManager::LoadTexture("../res/textures/background.jpg", true, "background");
    ResourceManager::LoadTexture("../res/textures/block.png", true, "block");
    ResourceManager::LoadTexture("../res/textures/block_solid.png", true, "block_solid");

    // load Levels
    size_t level_count        = 4;
    m_Levels                  = std::vector<GameLevel>(level_count);
    const char *level_names[] = {
        "1_standard",
        "2_a_fw_small_gaps",
        "3_space_invader",
        "4_bounce_galore",
    };
    for (int i = 0; i < level_count; ++i)
    {
        m_Levels[i].Load(fmt::format("../res/levels/{}", level_names[i]).c_str(), m_Width, m_Height / 2);
    }

    m_LevelIndex = 0;


    GL_CHECK_HEALTH();
}

void Game::Update(float dt)
{
}

void Game::ProcessInput(float dt)
{
}

void Game::Render()
{
    // Sprites[shader].DrawSprite(ResourceManager::GetTexture("arch"),
    //                            glm::vec2(200.f, 200.f),
    //                            glm::vec2(200.f, 300.f),
    //                            45.f,
    //                            glm::vec3(1.f, 1.f, 0.f));

    // if (this->m_State == GameState::GAME_ACTIVE) {
    //     // draw BG
    //     Sprites[shader].DrawSprite(ResourceManager::GetTexture("background"),
    //                                glm::vec2(0.f, 0.f),
    //                                glm::vec2(m_Width, m_Height),
    //                                0.f);
    //     // draw level
    //     m_Levels[m_LevelIndex].Draw(Sprites[shader]);
    // }

    // m_Level.Draw(Sprites[shader]);

    // it will draw in the sprite
    // dbgDraw();
}


void Game::dbgDraw()
{
    glPushMatrix();
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    {
        glVertex2f(0, 800);
        glVertex2f(800, 0);

        glVertex2f(-1, -1);
        glVertex2f(1, 1);
    }
    glEnd();
    glPopMatrix();
}
