#include "game.h"
#include "GLFW/glfw3.h"
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "obj/ball_object.h"
#include "obj/game_object.h"
#include <cmath>
#include <filesystem>
#include <functional>
#include <gl_macros.h>

#include "post_processing/post_processor.h"


#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/mat4x4.hpp>
#include <math.h>
#include <memory>
#include <mutex>
#include <stdlib.h>
#include <string_view>
#include <tuple>
#include <vector>

#include <fmt/format.h>
#include <log.h>



static auto sprite_shader   = "sprite";
static auto particle_shader = "particle";

template <class T>
static auto                                   clamp(T val, T min, T max) -> T { std::max(min, std::min(max, val)); }
static Direction                              VectorDirection(glm::vec2 target);
static bool                                   CollisionCheck_2Rect_AABB(const GameObject &A, const GameObject &B);
static std::tuple<bool, Direction, glm::vec2> CollisionCheck_BallWithRect(const BallObject &A, const GameObject &B);
static bool                                   ShouldSpawn(unsigned int change);



Game::Game(unsigned int width, unsigned int height) : m_State(GameState::GAME_ACTIVE), m_keys(), m_Width(width), m_Height(height)
{
}

Game::~Game()
{
}

void Game::Init()
{
    LOG_LOG("W: {} | H: {}", m_Width, m_Height);

    // loas shader
    ResourceManager::LoadShader("../res/shaders/a.vert", "../res/shaders/a.frag", nullptr, sprite_shader);
    ResourceManager::LoadShader("../res/shaders/particle.vert", "../res/shaders/particle.frag", nullptr, particle_shader);
    ResourceManager::LoadShader("../res/shaders/post_processing.vert", "../res/shaders/post_processing.frag", nullptr, "post_processing");

    // view projection to resolute the [-1,1]
    glm::mat4 projection = glm::ortho(0.0f, (float)this->m_Width,
                                      (float)this->m_Height, 0.0f,
                                      1.0f, -1.0f);

    // NOTICE: Must use this program first
    ResourceManager::GetShader(sprite_shader).Use();
    ResourceManager::GetShader(sprite_shader).SetMatrix4("projection", projection);
    // reset image
    ResourceManager::GetShader(sprite_shader).SetInteger("image", 0);
    ResourceManager::GetShader(sprite_shader).SetInteger("hasTexture", 1);

    m_SpriteRnder = SpriteRender(ResourceManager::GetShader(sprite_shader));

    ResourceManager::GetShader(particle_shader).Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader(particle_shader).SetInteger("sprite", 0);


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
    m_Player             = std::make_shared<GameObject>(player_pos, PLAYER_SIZE,
                                            ResourceManager::GetTextureRef("paddle"));

    // Ball
    auto ball_pos = player_pos + glm::vec2(PLAYER_SIZE.x / 2.f - BALL_RADIUS,
                                           -BALL_RADIUS * 2.f);
    m_Ball        = std::make_shared<BallObject>(ball_pos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTextureRef("face"));

    // Particles
    m_Particles = std::make_shared<ParticleGenerator>(ResourceManager::GetShader(particle_shader),
                                                      ResourceManager::GetTexture("arch"),
                                                      500);


    // post processing
    m_Effects = std::shared_ptr<PostProcessor>(new PostProcessor(ResourceManager::GetShader("post_processing"),
                                                                 m_Width, m_Height));

    GL_CHECK_HEALTH();
}

void Game::Update(float dt)
{
    if (ShakeTime > 0.f) {
        ShakeTime -= dt;
        if (ShakeTime < 0.f)
            m_Effects->bShake = false;
    }


    // ball with collisions
    m_Ball->Move(dt, m_Width);

    DoCollisions();
    if (m_Ball->m_Position.y >= m_Height) {
        ResetLevel();
        ResetPlayer();
    }

    // particles
    m_Particles->Update(dt, *m_Ball, 2, glm::vec2(m_Ball->m_Radius / 2.f));
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
                if (m_Ball->bStuck) {
                    m_Ball->m_Position.x -= velocity;
                }
            }
        }
        if (this->m_keys[GLFW_KEY_D]) {
            if (m_Player->m_Position.x <= m_Width - m_Player->m_Size.x)
            {
                m_Player->m_Position.x += velocity;

                // give a start force to ball
                if (m_Ball->bStuck) {
                    m_Ball->m_Position.x += velocity;
                }
            }
        }

        if (m_keys[GLFW_KEY_SPACE]) {
            m_Ball->bStuck = false;
        }
    }
}

void Game::Render()
{
    if (this->m_State == GameState::GAME_ACTIVE)
    {
        m_Effects->BeginRender();
        {
            m_SpriteRnder.DrawSprite(ResourceManager::GetTextureRef("background"),
                                     glm::vec2(0.f, 0.f),
                                     glm::vec2(m_Width, m_Height),
                                     0.f);
            m_Levels[m_LevelIndex].Draw(m_SpriteRnder);
            m_Player->Draw(m_SpriteRnder);
            // render the particles before the ball, because it use ONE as blend
            m_Particles->Draw();
            m_Ball->Draw(m_SpriteRnder);
        }
        m_Effects->EndRender();
        m_Effects->Render(glfwGetTime());
    }

    // debugDraw();
}

void Game::DoCollisions()
{
    // Ball with bricks
    for (auto &box : m_Levels[m_LevelIndex].Bricks) {
        if (!box.m_IsDestroyed) {
            auto &&[collided, dir_to_ball, diff] = CollisionCheck_BallWithRect(*m_Ball, box);

            if (collided)
            {
                // solid do shake
                if (box.m_IsSolid) {
                    ShakeTime         = 0.05f;
                    m_Effects->bShake = true;
                }
                else {
                    box.m_IsDestroyed = true;
                    this->SpawPowerUps(box);
                }


                float penetration = m_Ball->m_Radius - std::abs(diff.x);

                if (dir_to_ball == LEFT || dir_to_ball == RIGHT) {
                    m_Ball->m_Velocity.x = -m_Ball->m_Velocity.x;
                    // reloacte
                    if (dir_to_ball == LEFT)
                        m_Ball->m_Position.x += penetration;
                    else
                        m_Ball->m_Position.x -= penetration;
                }
                else {
                    m_Ball->m_Velocity.y = -m_Ball->m_Velocity.y;
                    // reloacte on the
                    if (dir_to_ball == DOWN)
                        m_Ball->m_Position.y += penetration;
                    else
                        m_Ball->m_Position.y -= penetration;
                }
            }
        }
    }

    // Ball with player paddle
    auto [colided, dir, diff] = CollisionCheck_BallWithRect(*m_Ball, *m_Player);
    if (!m_Ball->bStuck && colided) {
        m_Ball->bStuck = m_Ball->bSticky;

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

    // The generate powerups with ball
    for (auto &power_up : m_PowerUps) {
        if (!power_up.m_IsDestroyed) {
            if (power_up.m_Position.y >= m_Height) {
                power_up.m_IsDestroyed = true;
            }
            if (CollisionCheck_2Rect_AABB(*m_Player, power_up)) {
                ActivatePowerups(power_up);
                power_up.m_IsDestroyed = true;
                power_up.bActivated    = true;
            }
        }
    }
}

void Game::ResetLevel()
{
    // I have preload this, will cause performance issue?
    m_Levels[m_LevelIndex].Reset(m_Width, m_Height / 2.f);
}

void Game::ResetPlayer()
{
    m_Player->m_Size     = PLAYER_SIZE;
    m_Player->m_Position = glm::vec2(m_Width / 2.f - m_Player->m_Size.x / 2.f,
                                     m_Height - PLAYER_SIZE.y);
    m_Ball->Reset(m_Player->m_Position + glm::vec2(PLAYER_SIZE.x / 2.f - BALL_RADIUS, -(BALL_RADIUS * 2.f)),
                  INITIAL_BALL_VELOCITY);
}



void Game::SpawPowerUps(GameObject &block)
{
    static auto
        tex_speed             = ResourceManager::GetTexture("powerup_speed"),
        tex_stick             = ResourceManager::GetTexture("powerup_stick"),
        tex_pass_through      = ResourceManager::GetTexture("powerup_passthough"),
        tex_pad_size_increase = ResourceManager::GetTexture("powerup_increase"),
        tex_confuse           = ResourceManager::GetTexture("powerup_confuse"),
        tex_chaos             = ResourceManager::GetTexture("powerup_chaos");


    if (ShouldSpawn(75)) {
        m_PowerUps.push_back(PowerUp("speed", glm::vec3(0.5f, 0.5f, 1.0f),
                                     0.f, block.m_Position, tex_speed));
    };
    if (ShouldSpawn(75)) {
        m_PowerUps.push_back(PowerUp("stick", glm::vec3(1.0f, 0.5f, 1.0f),
                                     20.f, block.m_Position, tex_stick));
    };
    if (ShouldSpawn(75)) {
        m_PowerUps.push_back(PowerUp("pass_through", glm::vec3(0.5f, 1.0f, 0.5f),
                                     10.f, block.m_Position, tex_pass_through));
    };
    if (ShouldSpawn(75)) {
        m_PowerUps.push_back(PowerUp("pad_size_increse", glm::vec3(1.0f, 0.6f, 0.4f),
                                     0.f, block.m_Position, tex_pad_size_increase));
    }
    if (ShouldSpawn(15)) {
        m_PowerUps.push_back(PowerUp("confuse", glm::vec3(1, 0.3, 0.3),
                                     15.f, block.m_Position, tex_confuse));
    };
    if (ShouldSpawn(15)) {
        m_PowerUps.push_back(PowerUp("chaos", glm::vec3(0.9, 0.25, 0.25),
                                     15.f, block.m_Position, tex_chaos));
    }
}

void Game::UpdatePowers(float dt)
{
}

void Game::debugDraw()
{
}

void Game::ActivatePowerups(PowerUp &power_up)
{
    if (power_up.Type == "speed")
    {
        m_Ball->m_Velocity *= 1.2;
    }
    else if (power_up.Type == "sticky")
    {
        m_Ball->bSticky   = true;
        m_Player->m_Color = glm::vec3(1.0f, 0.5f, 1.0f);
    }
    else if (power_up.Type == "pass_through")
    {
        m_Ball->bPassThrough = true;
        m_Ball->m_Color      = glm::vec3(1.0f, 0.5f, 0.5f);
    }
    else if (power_up.Type == "pad_size_increase")
    {
        m_Player->m_Size.x += 50;
    }
    else if (power_up.Type == "confuse")
    {
        if (!m_Effects->bChaos)
            m_Effects->bConfuse = true; // only activate if chaos wasn't already active
    }
    else if (power_up.Type == "chaos")
    {
        if (!m_Effects->bConfuse)
            m_Effects->bChaos = true;
    }
}

// -------------------------

static bool CollisionCheck_2Rect_AABB(const GameObject &A, const GameObject &B)
{
    bool collisionX = A.m_Position.x + A.m_Size.x >= B.m_Position.x &&
                      B.m_Position.x + B.m_Size.x >= A.m_Position.x;

    bool collisionY = A.m_Position.y + A.m_Size.y >= B.m_Position.y &&
                      B.m_Position.y + B.m_Size.y >= A.m_Position.y;

    return collisionX && collisionY;
}

// from dot product get the projection of Vec to 4 diretions to get the longest vec,
// which is the best_match direction
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

static std::tuple<bool, Direction, glm::vec2> CollisionCheck_BallWithRect(const BallObject &A, const GameObject &B)
{
    using glm::vec2;

    // get the center (from topleft or circle and rectangle)
    vec2 ball_center(A.m_Position + A.m_Radius);

    vec2       rect_center{};
    const vec2 rect_half_extens(B.m_Size.x / 2.f, B.m_Size.y / 2.f);
    rect_center = B.m_Position + rect_half_extens;

    vec2 diff = ball_center - rect_center;

    // it will let the x or y of vec from circle to rectangle
    // which beyond the half rectangle's L or W
    // clmaped to point to  the edge of rectangle (from rectangle center)
    // so it is the cloest point from rectangle to the circle
    vec2 clamped       = clamp(diff, -rect_half_extens, rect_half_extens);
    vec2 closeet_point = rect_center + clamped;

    // retrieve vec between center circle and closest point AABB
    //  and check if  len < radius
    diff = closeet_point - ball_center;

    if (glm::length(diff) <= A.m_Radius)
        return {true, VectorDirection(diff), diff};
    else
        return {
            false, UP, {0.f, 0.f}
        };
}

static bool ShouldSpawn(unsigned int change)
{
    return rand() % change == 0;
}
