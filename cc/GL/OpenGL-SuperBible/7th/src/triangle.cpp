#include <application.h>
#include <gl_macros.h>
#include <glm/common.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <log.h>
#include <math.h>
#include <resource_manager/resource_manager.h>

static void debug_pts()
{
    glPointSize(15.f);
    glBegin(GL_POINTS);
    glVertex2f(-0.5f, 0.5f);
    glVertex2f(0, 0);
    glEnd();
}

class Triangle : public Application
{
  public:
    GLuint VAO, VBO;
    void   Construct() override
    {
        Super::Construct();
        LOG("Triangle contract...");
    }

    void BeginPlay() override
    {
        Super::BeginPlay();

        ResourceManager::LoadShader("../res/shaders/sprite/sprite.vert", "../res/shaders/sprite/sprite.frag", nullptr, "Block");

        float vertices[] =
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



    void OnEvent() override
    {
        Super::OnEvent();
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

int main()
{
    Triangle t;
    t.Run();
}
