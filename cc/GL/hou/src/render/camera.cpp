//
// Created by nono on 10/12/23.
//

#include "camera.h"
Camera::Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 world_up)
{
    pos      = pos;

    forward  = glm::normalize(target - pos);
    world_up = world_up;
    right    = glm::cross(forward, world_up);
    up       = glm::normalize(glm::cross(right, forward));
}
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(pos, pos + forward, world_up);
}
