
#include "ball_object.h"
#include "game_object.h"
#include "glm/detail/qualifier.hpp"
#include "glm/fwd.hpp"
#include <cmath>
#include <math.h>


BallObject::BallObject() {}
BallObject::BallObject(glm::vec2 pos, float Radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(Radius * 2, Radius * 2), sprite)
{
    m_Velocity = velocity;
    m_Struck   = true;
    m_Radius   = Radius;
}

glm::vec2 BallObject::Move(float dt, int window_width)
{
    if (!m_Struck) {
        m_Position += m_Velocity * dt;

        if (m_Position.x < 0.f) {
            m_Velocity.x = -m_Velocity.x;
            m_Position.x = 0.f;
        }
        else if (m_Position.x + m_Size.x >= window_width) {
            m_Velocity.x = -m_Velocity.x;
            m_Position.x = window_width - m_Size.x;
        }
        else if (m_Position.y < 0.f) {
            m_Velocity.y = -m_Velocity.y;
            m_Position.y = 0.f;
        }
    }
    return this->m_Position;
}

void BallObject::Reset(glm::vec2 pos, glm::vec2 velocity)
{
    m_Position     = pos;
    m_Velocity     = velocity;
    this->m_Struck = true;
}
