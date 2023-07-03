#include "game.h"
#include "glm/geometric.hpp"
#include "render/sprite_render.h"
#include <cstddef>
#include <filesystem>
#include <functional>
#include <gl_macros.h>

#include "resource_manager/resource_manager.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>
#include <math.h>
#include <mutex>
#include <string_view>
#include <unistd.h>
#include <vector>

#include <fmt/format.h>
#include <log.h>



static auto shader = "sprite";

Game::Game(unsigned int width, unsigned int height) : m_State(GameState::GAME_ACTIVE), m_keys(), m_Width(width), m_Height(height) {}

Game::~Game() {}

void Game::Init()
{
    LOG_LOG("W: {} | H: {}", m_Width, m_Height);

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

    SpriteRenders[shader] = SpriteRender(ResourceManager::GetShader(shader));



    // load texture
    {
#if 1
        auto GetFileNameWithoutExtension = [](const std::string &path) {
            size_t slash_pos = path.find_last_of("/\\");
            size_t dot_pos   = path.find_last_of(".");

            // DEBUG("{}, {}, {}", path, slash_pos, dot_pos);
            auto filename = path.substr(slash_pos + 1, dot_pos - slash_pos - 1);
            // LOG("{}", filename);
            return filename;
        };

        std::vector<std::string> support_suffixs = {".jpg", ".png", ".bmp"};
        LOG_WARN("Load texture with '{}', '{}', '{}'", support_suffixs[0], support_suffixs[1], support_suffixs[2]);

        std::mutex mtx;
        for (const auto texture : std::filesystem::directory_iterator("../res/textures/"))
        {
            const auto &file_path = texture.path().string();
            for (auto &suffx : support_suffixs)
            {
                if (file_path.ends_with(suffx)) {
                    const auto texture_name = GetFileNameWithoutExtension(std::ref(file_path));
                    // LOG_DEBUG("__{}__", texture_name);
                    std::lock_guard<std::mutex> lg(mtx);
                    ResourceManager::LoadTexture(file_path.c_str(), true, texture_name);
                }
            }
        }
#else
        // ResourceManager::LoadTexture("../res/textures/arch.png", true, "arch");
        // ResourceManager::LoadTexture("../res/textures/brick.bmp", true, "brick");
        // ResourceManager::LoadTexture("../res/textures/block_solid.png", true, "block_solid");
        // ResourceManager::LoadTexture("../res/textures/block.png", true, "block");
        // ResourceManager::LoadTexture("../res/textures/background.jpg", true, "background");
#endif
    }



    // load Levels
    size_t level_count        = 4;
    m_Levels                  = std::vector<GameLevel>(level_count);
    const char *level_names[] = {
        "1_standard",
        "2_a_few_small_gaps",
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

    debugDraw();
}


void Game::debugDraw()
{
    SpriteRenders[shader].DrawSprite(ResourceManager::GetTextureRef("arch"),
                                     glm::vec2(200, 0),
                                     glm::vec2(200.f, 200.f),
                                     45.f);

    SpriteRenders[shader].DrawSprite(ResourceManager::GetTextureRef("brick"),
                                     glm::vec2(200.f, 200.f),
                                     glm::vec2(200.f, 200.f));

    SpriteRenders[shader].DrawSprite(ResourceManager::GetTextureRef("block_solid"),
                                     glm::vec2(400.f, 400.f),
                                     glm::vec2(200.f, 200.f));
    SpriteRenders[shader].DrawSprite(ResourceManager::GetTextureRef("background"),
                                     glm::vec2(0, 0),
                                     glm::vec2(100, 100));
    SpriteRenders[shader].DrawSprite(ResourceManager::GetTextureRef("block"),
                                     glm::vec2(0.f, 200.f),
                                     glm::vec2(200.f, 200.f));


    // it will draw in the sprite
    glPushMatrix();
    {
        glColor3f(1, 1, 1);
        glBegin(GL_LINES);
        {
            glVertex2f(0, 800);
            glVertex2f(800, 0);

            glVertex2f(-1, -1);
            glVertex2f(1, 1);
        }
        glEnd();
    }
    glPopMatrix();
}
