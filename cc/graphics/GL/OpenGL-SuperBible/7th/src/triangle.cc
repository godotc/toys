#include "application_sdl.h"
#include "fmt/core.h"
#include "gl_macros.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/constants.hpp"
#include "log.h"
#include "path.hpp"
#include "resource_manager/resource_manager.h"
#include "utils/gl_utils.h"
#include <cstddef>
#include <cstdio>
#include <glm/matrix.hpp>
#include <iostream>
#include <math.h>
#include <vector>


struct VAO {
    GLuint vertex_array_object;

    VAO()
    {
        glCreateVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);
    }

    void bind()
    {
        glBindVertexArray(vertex_array_object);
    }

    ~VAO()
    {
        glDeleteVertexArrays(1, &vertex_array_object);
    }
};

struct VAOAttr {
};

struct Buffer {
    GLuint       buffer;
    const GLenum buffer_type;

    Buffer(GLenum buffer_type) : buffer_type(buffer_type)
    {
        glCreateBuffers(1, &buffer);
        glBindBuffer(buffer_type, buffer);
    }


    void bind()
    {
        // use GL_ARRAY_BUFFER binding point
        glBindBuffer(buffer_type, buffer);
    }
};


class Triangle : public App
{
  protected:
    VAO    vao;
    Buffer vbo = Buffer(GL_ARRAY_BUFFER);

    glm::mat4 mv_mat   = glm::mat4(1.f),
              proj_mat = glm::mat4(1.f);

    virtual void init() override
    {
        Super::init();

        glPointSize(20);

        ResourceManager::LoadShader(FPath("//res/shaders/test.glsl"), "test");

        static const float vertices[] =
            {
				0.5, 0.5, -0.5f,
				0.5, -0.5, -0.5f,
				-0.5, -0.5, -0.5f,
				-0.5, 0.5, -0.5f,

				0.5, 0.5, 0.5f,
				0.5, -0.5, 0.5f,
				-0.5, -0.5, 0.5f,
				-0.5, 0.5, 0.5f,
			};



        vao.bind();
        vbo.bind();
        glBufferData(vbo.buffer_type, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);



        float aspect = m_Width / (float)m_Height;
        proj_mat     = glm::perspective(50.f, aspect, 0.1f, 1000.f);

        GL_CHECK_HEALTH();
    };

    virtual void update(float dt) override
    {
        Super::update(dt);

        float f = current_frame * 3.1415f * 0.1f;

        mv_mat = glm::rotate(mv_mat, current_frame * 45.f, glm::vec3(0.f, 1.f, 0.f));
        mv_mat = glm::rotate(mv_mat, current_frame * 81.f, glm::vec3(1.f, 0.f, 0.f));
        mv_mat = glm::translate(mv_mat, glm::vec3(0.f, 0.f, -4.f));
        mv_mat = glm::translate(mv_mat, glm::vec3(sinf(2.1f * f) * 0.5f,
                                                  cos(1.7f * f) * 0.5f,
                                                  sin(1.3f * f) * 0.5f * cosf(1.5f * f) * 2.f));
    };

    virtual void render(float dt) override
    {
        Super::render(dt);

        static const GLfloat red[]     = {1.f, 0, 0, 1};
        static const GLfloat my_cyan[] = {0.2f, 0.3, 0.5, 1};
        glClearBufferfv(GL_COLOR, 0, my_cyan);

        ResourceManager::GetShader("test")
            .Use()
            .SetMatrix4("mv_mat", mv_mat)
            .SetMatrix4("proj_mat", proj_mat);

        vao.bind();
        vbo.bind();

        glDrawArrays(GL_TRIANGLES, 0, 3);


        GL_CHECK_HEALTH();
    };
};


int main()
{
    Path::SetProjectRootSymbol("res");
    LOG_DEBUG("Project root:{}", Path::ProjectRoot());
    Triangle t;
    gltk::GLExtensions::Get().print_all_extensions();
    t.Run();
}
