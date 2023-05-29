#pragma once

#include "shader.h"
#include "texture.h"
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>

class SpriteRender
{
  public:
    SpriteRender() = default;
    SpriteRender(Shader &_shader)
    {
        this->shader = _shader;
        this->initRenderData();
    }

    ~SpriteRender()
    {
        glDeleteVertexArrays(1, &this->quadVAO);
    }

    void DrawSprite(
        Texture2D &texture,
        glm::vec2  position,
        glm::vec2  size   = glm::vec2(10.f, 10.f),
        float      rotate = 0.f,
        glm::vec3  color  = glm::vec3(1.f))
    {
        this->shader.Use();
        glm::mat4 model(1.0);
        model = glm::translate(model, glm::vec3(position, 0.f));

        model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.f));
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0, 0, 1));
        model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.f));

        model = glm::scale(model, glm::vec3(size, 1.f));

        this->shader.SetMatrix4("model", model);
        this->shader.SetVector3f("spriteColor", color);

        glActiveTexture(GL_TEXTURE0);
        texture.Bind();

        glBindVertexArray(this->quadVAO);
        {
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        glBindVertexArray(0);
    }

  private:
    Shader       shader;
    unsigned int quadVAO;

    void initRenderData()
    {
        unsigned int VBO;
        float        vertices[] =
            {0.0f, 1.0f, 0.0f, 1.0f,
             1.0f, 0.0f, 1.0f, 0.0f,
             0.0f, 0.0f, 0.0f, 0.0f,

             0.0f, 1.0f, 0.0f, 1.0f,
             1.0f, 1.0f, 1.0f, 1.0f,
             1.0f, 0.0f, 1.0f, 0.0f};

        glGenVertexArrays(1, &this->quadVAO);
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->quadVAO);
        {
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};
