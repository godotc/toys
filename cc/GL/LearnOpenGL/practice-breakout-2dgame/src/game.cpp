#include "game.h"
#include "GLFW/glfw3.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "obj/ball_object.h"
#include "obj/game_object.h"
#include "render/sprite_render.h"
#include <algorithm>
#include <cmath>
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
#include <tuple>
#include <vector>

#include <fmt/format.h>
#include <log.h>



static auto shader = "sprite";

template <class T>
static auto clamp(T val, T min, T max) -> T { std::max(min, std::min(max, val)); }

static bool AABBCheckCollision(const GameObject &A, const GameObject &B)
{
    bool collisionX = A.m_Position.x + A.m_Size.x >= B.m_Position.x &&
                      B.m_Position.x + B.m_Size.x >= A.m_Position.x;

    bool collisionY = A.m_Position.y + A.m_Size.y >= B.m_Position.y &&
                      B.m_Position.y + B.m_Size.y >= A.m_Position.y;

    return collisionX && collisionY;
}
static Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[4] = {
        { 0.f,  1.f},
        { 1.f,  0.f},
        { 0.f, -1.f},
        {-1.f,  0.f},
    };
    float        max        = 0.f;
    unsigned int best_match = -1;
    for (int i = 0; i < 4; ++i) {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max) {
            max        = dot_product;
            best_match = i;
        }
    }
    return static_cast<Direction>(best_match);
}

static std::tuple<bool, Direction, glm::vec2> CheckCollision(const BallObject &A, const GameObject &B)
{
    using glm::vec2;

    // get the center (from topleft or circle and rectangle)
    vec2 center(A.m_Position + A.m_Radius);
    vec2 aabb_half_extens(B.m_Size.x / 2.f, B.m_Size.y / 2.f);
    vec2 aabb_center(B.m_Position.x + aabb_half_extens.x,
                     B.m_Position.y + aabb_half_extens.y);

    vec2 difference = center - aabb_center;

    // it will let the x or y of vec from circle to rectangle
    // which beyond the half rectangle's L or W
    // clmaped to point to  the edge of rectangle (from rectangle center)
    // so it is the cloest point from rectangle to the circle
    vec2 clamped = clamp(difference, -aabb_half_extens, aabb_half_extens);
    vec2 closeet = aabb_center + clamped;

    // retrieve vec between center circle and closest point AABB
    //  and check if  len < radius
    difference = closeet - center;
    if (glm::length(difference) <= A.m_Radius)
        return {true, VectorDirection(difference), difference};
    else
        return {
            false, UP, {0.f, 0.f}
        };
}



Game::Game(unsigned int width, unsigned int height) : m_State(GameState::GAME_ACTIVE), m_keys(), m_Width(width), m_Height(height)
{
}

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
        auto GetFileNameWithoutExtension =
            [](const std::string &path) {
                size_t slash_pos = path.find_last_of("/\\");
                size_t dot_pos   = path.find_last_of(".");

                // DEBUG("{}, {}, {}", path, slash_pos, dot_pos);
                auto filename = path.substr(slash_pos + 1, dot_pos - slash_pos - 1);
                // LOG("{}", filename);
                return filename;
            };

        std::vector<std::string> support_suffixs = {".jpg", ".png", ".bmp"};
        LOG_WARN("Load texture with '{}', '{}', '{}'", support_suffixs[0], support_suffixs[1], support_suffixs[2]);

        for (const auto texture : std::filesystem::directory_iterator("../res/textures/"))
        {
            const auto &file_path = texture.path().string();
            // LOG_DEBUG("Trying to load texture from '{}'", file_path);

            for (auto &suffx : support_suffixs)
            {
                if (file_path.ends_with(suffx))
                {
                    LOG_TRACE("Trying to load texture from '{}'", file_path);
                    const std::string texture_name = GetFileNameWithoutExtension(std::ref(file_path));

                    ResourceManager::LoadTexture(file_path.c_str(), texture_name);
                }
            }
        }
#else
        ResourceManager::LoadTexture("../res/textures/arch.png", "arch");
        ResourceManager::LoadTexture("../res/textures/brick.bmp", "brick");
        ResourceManager::LoadTexture("../res/textures/block_solid.png", "block_solid");
        ResourceManager::LoadTexture("../res/textures/block.png", "block");
        ResourceManager::LoadTexture("../res/textures/background.jpg", "background");
#endif
    }

    // load Levels
    {
        size_t level_count = 4;
        m_Levels           = std::vector<GameLevel>(level_count);
        const char *level_names[] =
            {
                "0_standard",
                "1_a_few_small_gaps",
                "2_space_invader",
                "3_bounce_galore",
            };
        for (int i = 0; i < level_count; ++i)
        {
            m_Levels[i].Load(fmt::format("../res/levels/{}", level_names[i]).c_str(),
                             m_Width,
                             m_Height / 2);
        }
        m_LevelIndex = 0;
    }

    // Player
    glm::vec2 player_pos = glm::vec2(m_Width / 2.f - PLAYER_SIZE.x / 2.f,
                                     m_Height - PLAYER_SIZE.y);

    m_Player = std::make_shared<GameObject>(player_pos, PLAYER_SIZE,
                                            ResourceManager::GetTextureRef("paddle"));

    // Ball
    auto ball_pos = player_pos + glm::vec2(PLAYER_SIZE.x / 2.f - BALL_RADIUS,
                                           -BALL_RADIUS * 2.f);

    m_Ball = std::make_shared<BallObject>(ball_pos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTextureRef("face"));


    GL_CHECK_HEALTH();
}

void Game::Update(float dt)
{
    static float time = 0;
    time += dt;

    /*
         LOG_DEBUG("{}", time);
         if (time > 2)
         {
             time -= 2;
             m_LevelIndex = (m_LevelIndex + 1) % m_Levels.size();
        }
    */

    m_Ball->Move(dt, m_Width);
    DoCollisions();

    if (m_Ball->m_Position.y >= m_Height) {
        ResetLevel();
        ResetPlayer();
    }
}

void Game::ProcessInput(float dt)
{
    if (this->m_State == GameState::GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;

        if (this->m_keys[GLFW_KEY_A]) {
            if (m_Player->m_Position.x >= 0.f) {
                m_Player->m_Position.x -= velocity;

                // give a start force to ball
                if (m_Ball->m_Struck) {
                    m_Ball->m_Position.x -= velocity;
                }
            }
        }
        if (this->m_keys[GLFW_KEY_D]) {
            if (m_Player->m_Position.x <= m_Width - m_Player->m_Size.x)
            {
                m_Player->m_Position.x += velocity;

                // give a start force to ball
                if (m_Ball->m_Struck) {
                    m_Ball->m_Position.x += velocity;
                }
            }
        }

        if (m_keys[GLFW_KEY_SPACE]) {
            m_Ball->m_Struck = false;
        }
    }
}

void Game::Render()
{
    if (this->m_State == GameState::GAME_ACTIVE) {
        // draw BG
        SpriteRenders[shader].DrawSprite(ResourceManager::GetTextureRef("background"),
                                         glm::vec2(0.f, 0.f),
                                         glm::vec2(m_Width, m_Height),
                                         0.f);
        // draw level
        m_Levels[m_LevelIndex].Draw(SpriteRenders[shader]);
        m_Player->Draw(SpriteRenders[shader]);
        m_Ball->Draw(SpriteRenders[shader]);
    }

    // debugDraw();
}

void Game::DoCollisions()
{
    // Ball with bricks
    for (auto &box : m_Levels[m_LevelIndex].Bricks) {
        if (!box.m_IsDestroyed)
        {
            auto &&[collided, dir, diff] = CheckCollision(*m_Ball, box);
            if (collided)
            {
                if (!box.m_IsSolid)
                    box.m_IsDestroyed = true;

                if (dir == LEFT || dir == RIGHT) {
                    m_Ball->m_Velocity.x = -m_Ball->m_Velocity.x;
                    // reloacte
                    float penetration = m_Ball->m_Radius - std::abs(diff.x);
                    if (dir == LEFT)
                        m_Ball->m_Position.x += penetration;
                    else
                        m_Ball->m_Position.x -= penetration;
                }
                else {
                    m_Ball->m_Velocity.y = -m_Ball->m_Velocity.y;
                    // reloacte
                    float penetration = m_Ball->m_Radius - std::abs(diff.y);
                    if (dir == LEFT)
                        m_Ball->m_Position.y += penetration;
                    else
                        m_Ball->m_Position.y -= penetration;
                }
            }
        }
    }

    // Ball with player paddle
    auto [colided, dir, diff] = CheckCollision(*m_Ball, *m_Player);
    if (!m_Ball->m_Struck && colided) {
        // if collided, we only need to caculate the hit point, and not the dir vec
        float center_board = m_Player->m_Position.x + m_Player->m_Size.x / 2.f;
        float distance     = (m_Ball->m_Position.x + m_Ball->m_Radius) - center_board;
        float percentage   = distance / (m_Player->m_Size.x / 2.f);

        float strength       = 2.f;
        auto  old_velocity   = m_Ball->m_Velocity;
        m_Ball->m_Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;

        // make the ball not stick when just in the paddle
        // m_Ball->m_Velocity.y = -m_Ball->m_Velocity.y;
        m_Ball->m_Velocity.y = -1.f * abs(m_Ball->m_Velocity.y);

        m_Ball->m_Velocity = glm::normalize(m_Ball->m_Velocity) * glm::length(old_velocity);
    }
}

void Game::ResetLevel()
{
    // I have preload this, will cause performance issue?
    m_Levels[m_LevelIndex].Reset(m_Width, m_Height / 2.f);

    // if (this->Level == 0)
    //     this->Levels[0].Load("levels/one.lvl", this->Width, this->Height / 2);
    // else if (this->Level == 1)
    //     this->Levels[1].Load("levels/two.lvl", this->Width, this->Height / 2);
    // else if (this->Level == 2)
    //     this->Levels[2].Load("levels/three.lvl", this->Width, this->Height / 2);
    // else if (this->Level == 3)
    //     this->Levels[3].Load("levels/four.lvl", this->Width, this->Height / 2);
}

void Game::ResetPlayer()
{
    m_Player->m_Size     = PLAYER_SIZE;
    m_Player->m_Position = glm::vec2(m_Width / 2.f - m_Player->m_Size.x / 2.f,
                                     m_Height - PLAYER_SIZE.y);
    m_Ball->Reset(m_Player->m_Position + glm::vec2(PLAYER_SIZE.x / 2.f - BALL_RADIUS, -(BALL_RADIUS * 2.f)),
                  INITIAL_BALL_VELOCITY);
}
