
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include <math.h>
#include <stdexcept>

using std::cout, std::endl;

const float PI    = 3.1415926;
const int   steps = 100;

void drawCircle(glm::vec3 &&rgb)
{
    const float angle  = PI * 2 / steps;
    float       radius = 1;
    float       oldX = 0, oldY = 1;

    for (int i = 0; i <= steps; ++i) {
        float newX = radius * cos(angle * i);
        float newY = -radius * sin(angle * i);

        glColor3f(rgb.x, rgb.y, rgb.z);

        glBegin(GL_TRIANGLES);
        glVertex3f(0, 0, 0);
        glVertex3f(oldX, oldY, 0);
        glVertex3f(newX, newY, 0);
        glEnd();

        oldX = newX;
        oldY = newY;
    }
}



int main()
{
    std::cout << "hello world" << std::endl;

    GLFWwindow *p_window;

    // 1. init
    if (!glfwInit()) {
        throw std::runtime_error("glfw init error!");
    }

    // 2. create window
    p_window = glfwCreateWindow(800, 800, "Hello GLFW", 0, 0);
    if (!p_window) {
        throw std::runtime_error("create window error");
    }

    // 3. bind window context
    glfwMakeContextCurrent(p_window);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(0.1, 0.1, 1); // x,y shrink 10 times

    float angle     = 0;
    float angleMoon = 0;

    // 4. render loop
    while (!glfwWindowShouldClose(p_window)) {

        angle += 1;
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        // sum
        drawCircle(glm::vec3{1, 1, 0});

        {
            // earth
            glPushMatrix();
            glRotatef(angle, 0, 0, 1); // around z axis
            glTranslatef(0, 5, 0);
            glScalef(0.6, 0.6, 1);
            drawCircle(glm::vec3{0, 0.3, 1});

            angleMoon += 3;
            {
                // moon
                glPushMatrix();
                glRotatef(angleMoon, 0, 0, 1); // around z axis
                glTranslatef(0, 3, 0);
                glScalef(0.3, 0.3, 1);
                drawCircle(glm::vec3{0.3, 0.3, 0.3});
                glPopMatrix();
            }
            glPopMatrix();
        }

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }

    glfwTerminate();

    cout << "program closed" << endl;
    return 0;
}
