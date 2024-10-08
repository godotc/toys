#pragma once

#include "gl_macros.h"
#include "resource_manager/resource_manager.h"
#include <glm/common.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>



/**
 * @brief Sprite, reprensent a 2D image or Object in 2D screnn
 *
 */
class GLTK_API SpriteRender
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
        using std::cout, std::endl;
        // cout << "delete render" << endl;
        //  GL_CALL(glDeleteVertexArrays(1, &this->quadVAO));
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

        // scale <- rotate <- translate
        model = glm::translate(model, glm::vec3(position, 0.f));

        model = glm::translate(model, glm::vec3(0.5 * size.x, 0.5 * size.y, 0.f));
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(0, 0, 1));
        model = glm::translate(model, glm::vec3(-0.5 * size.x, -0.5 * size.y, 0.f));

        model = glm::scale(model, glm::vec3(size, 1.f));

        this->shader.SetMatrix4("model", model);
        this->shader.SetVector3f("spriteColor", color);

        GL_CALL(GL_CALL(glActiveTexture(GL_TEXTURE0)));
        texture.Bind();

        GL_CALL(glBindVertexArray(this->quadVAO));
        {
            GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
        }
        GL_CALL(glBindVertexArray(0));
    }

  private:

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

        GL_CALL(glGenVertexArrays(1, &this->quadVAO));
        GL_CALL(glGenBuffers(1, &VBO));

        GL_CALL(glBindVertexArray(this->quadVAO));

        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

        {
            GL_CALL(glEnableVertexAttribArray(0));
            GL_CALL(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0));
        }

        GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GL_CALL(glBindVertexArray(0));
    }

  private:
    Shader       shader;
    unsigned int quadVAO;
};
