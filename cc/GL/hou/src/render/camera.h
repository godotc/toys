//
// Created by nono on 10/12/23.
//

#ifndef HOU_CAMERA_H
#define HOU_CAMERA_H


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

struct Camera {
  public:
    Camera(glm::vec3 pos, glm::vec3 target, glm::vec3 world_up);
    glm::mat4 GetViewMatrix();

    glm::vec3 pos;

    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 world_up;
};



#endif // HOU_CAMERA_H
