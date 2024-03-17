#include "GLFW/glfw3.h"
#include "level.h"
#include <application_glfw.h>

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <gl_macros.h>
#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <log.h>
#include <math.h>
#include <ostream>
#include <random>
#include <resource_manager/resource_manager.h>
#include <stdlib.h>
#include <sys/types.h>


#include <time.h>

static void debug_pts()
{
    glPointSize(15.f);
    glBegin(GL_POINTS);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0, 0);
    glEnd(); // alawys erro
}
struct Celestial;


static const float G = 9.8f;

struct Celestial {
    float     Quality = 1.f;
    glm::vec3 Location;
    glm::vec3 Velocity;

    Celestial()
    {
#if _HAS_CXX17
        std::random_device                    rd;
        std::mt19937                          seed(rd());
        std::uniform_real_distribution<float> gen(-1, 1);

        Location.x = gen(seed);
        Location.y = gen(seed);
        Location.z = 0.f;

        std::uniform_real_distribution<float> gen2(-0.001, 0.001);

        Velocity.x = gen2(seed);
        Velocity.y = gen2(seed);
        Velocity.z = 0.f;
#else
        auto get_random_float = [](auto min, auto max) -> float {
            auto d = abs(max - min);
            return rand() / (float)RAND_MAX * d - max;
        };

        Location.x = get_random_float(-1, 1);
        Location.y = get_random_float(-1, 1);
        Location.z = 0.f;

        Velocity.x = get_random_float(-0.001, 0.001);
        Velocity.y = get_random_float(-0.001, 0.001);
        Velocity.z = 0.f;
#endif
        LOG_DEBUG("Initial Location: [{}, {}, {}]", Location.x, Location.y, Location.z);
        LOG_DEBUG("Initial Velocity: [{}, {}, {}]", Velocity.x, Velocity.y, Velocity.z);
    }

    auto UpdateVelocity(Celestial &other, float dt, bool all_the_same = true)
    {
        LOG_DEBUG("Velocity: [{}, {}, {}]", Velocity.x, Velocity.y, Velocity.z);

        auto v12 = other.Location - Location;
        // Get gravity between both celetial
        // 假设有一个万有引力公式：F = G * (m1 * m2) / (r^2)，其中 G 为引力常数

        float distance = std::max(0.001f, glm::length(v12));

        auto dir = glm::normalize(v12);
        // if (distance == 0.f)
        //     distance += 0.0000000001f;

        const float weight = 0.001f;

        float gravity{};
        gravity = weight * G * (Quality * other.Quality) / (distance * distance);

        auto increment = gravity * dir;

        Velocity += increment * dt;
    }


    auto Advance()
    {
        if (Location.x < -1 || Location.x > 1) {
            Velocity.x = -Velocity.x;
            // Location.x = glm::clamp(Location.x, -1.f, 1.f);
        }
        if (Location.y < -1 || Location.y > 1) {
            Velocity.y = -Velocity.y;
            // Location.y = glm::clamp(Location.y, -1.f, 1.f);
        }
        if (Location.z < -1 || Location.z > 1) {
            Velocity.z = -Velocity.z;
            // Location.z = glm::clamp(Location.z, -1.f, 1.f);
        }

        Location += Velocity;
    }
};


std::ostream &operator<<(std::ostream &out, Celestial &c)
{
    out << "{" << c.Location.x << " " << c.Location.y << " " << c.Location.z << "}\n";
    out << "{" << c.Velocity.x << " " << c.Velocity.y << " " << c.Velocity.z << "}\n";
    return out;
}

class TriBody_Intermediate : public App
{
  public:
    static const int N = 3;

    std::array<Celestial, N> bodys;

    void BeginPlay() override
    {
        GL_CALL(glPointSize(16.f));
        glClearColor(0, 0, 0, 1);
        glColor3f(0, 1, 0);
        glPointSize(15.f);
        glEnable(GL_POINT_SMOOTH);
        // glfwSetWindowShouldClose(m_Window, true);
    }

    // slow down this tick
    void Tick(uint32_t dt) override
    {
        Super::Tick(dt);

        static size_t time_count = 0;
        time_count += dt;

        if (time_count % (size_t)1e3)
        {
            int n = 10;
            dt /= (float)n;
            for (int i = 0; i < n; ++i) {

                glClear(GL_COLOR_BUFFER_BIT);

                for (int i = 0; i < N; ++i) {
                    for (int j = 0; j < N; ++j) {
                        if (i == j)
                            continue;
                        bodys[i].UpdateVelocity(bodys[j], dt);
                    }
                }

                for (auto &b : bodys) {
                    // std::cout << b << "-------------------------------------------------\n";
                    b.Advance();
                }

                draw();

                LOG_DEBUG("Current energy: {}", enegry());
            }
        }
    }

    /**
     * @brief  Get the inner energy of a system
     *  To check the energy is conservation or not
     * @return float
     */
    auto enegry() -> float
    {
        float potE = 0.f;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i == j)
                    continue;
                auto &b1 = bodys[i];
                auto &b2 = bodys[j];

                // 引力势能 Gravitational potential enegy (Negative)： E = -SUM (G * mi*mj/r_ij)
                potE += G * b1.Quality * b2.Quality / glm::length(b2.Location - b1.Location);
            }
        }

        float movE = 0.f;

        for (auto &b : bodys) {
            // 动能 Kinetic(Positive)： E = 0.5*m*v^2
            // m: quality
            // v: 质点运动速度
            auto v = glm::length(b.Velocity);
            movE += 0.5f * b.Quality * v * v;
        }

        return movE - potE;
    }


    void draw()
    {
        debug_pts();

        glBegin(GL_POINTS);
        for (auto &b : bodys) {
            glVertex3f(b.Location.x, b.Location.y, b.Location.z);
        }
        glEnd();
    }
};


class TriBody : public App
{
  public:
    GLuint           VAO, VBO;
    static const int NPoints = 3;
    void             Construct() override
    {
        Super::Construct();
        LOG_DEBUG("3Body contract...");
    }

    void BeginPlay() override
    {
        Super::BeginPlay();

        // ResourceManager::LoadShader("../res/shaders/sprite/sprite.vert", "../res/shaders/sprite/sprite.frag", nullptr, "Block");
        ResourceManager::LoadShader("../res/shaders/test.glsl", "Block");

        // TODO random startpoint and velocity
        std::mt19937 s;
        float        vertices[NPoints * 3] =
            {50, 50, 0.f,
             50, 100, 0,
             100, 50, 0};

        auto x = (float)glm::min(m_Width, m_Height);

        auto projection = glm::ortho(0.f, (float)m_Width, 0.f, (float)m_Height, -x, x);
        ResourceManager::GetShader("Block").Use().SetMatrix4("projection", projection);
        // ResourceManager::GetShader("Block").SetMatrix4("model", glm::mat4(1.f));



        GL_CALL(glGenVertexArrays(1, &VAO));
        GL_CALL(glBindVertexArray(VAO));

        GL_CALL(glGenBuffers(1, &VBO));
        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(float), vertices, GL_STATIC_DRAW));

        GL_CALL(glEnableVertexAttribArray(0));
        // GL_CALL(glVertexAttribPointer(0, sizeof(vertices), GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0)); // 1281 INVALID_ARGUMENT
        GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0)); // vec3
    }



    void Tick(uint32_t DeltaT) override
    {
        Super::Tick(DeltaT);

        GL_CALL(glClearColor(0, 0, 0, 0));
        GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

        ResourceManager::GetShader("Block").Use();
        ResourceManager::GetShader("Block").SetInteger("bPureColor", 1); // without texture image
        GL_CALL(glBindVertexArray(VBO));
        GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));

        debug_pts();
    }
};


int main(int argc, char **argv)
{

#if !_HAS_CXX17
    srand(time(nullptr));
#endif
    __logcpp::SetLogLevel(__logcpp::LogLevel::L_DEBUG);

    LOG_DEBUG("Hello World!");
    TriBody_Intermediate t;
    // TriBody t;
    t.Run();

    return 0;
}
