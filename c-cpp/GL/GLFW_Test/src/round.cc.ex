
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <stdexcept>

using std::cout, std::endl;

int steps = 4;
// 2 pie == 360
float angle = 3.1415926 * 2.f / steps;

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

    float xPos   = 0;
    float yPos   = 0;
    float radius = 1.0f;

    int tick = 0;

    // 4. render loop
    while (!glfwWindowShouldClose(p_window)) {
        ++tick;
        if (tick % 100 == 0) {
            if (steps < 200) {
                ++steps;
                angle = 3.1415926 * 2.f / steps;
            }
            cout << "tick 100 times" << endl;
            cout << "steps =" << steps << ", angle =" << angle << endl;
            tick -= 100;
        }


        glClearColor(0, 0, 1, 0);

        glClear(GL_COLOR_BUFFER_BIT);

        float prevX = xPos;
        float prevY = yPos;

        for (int i = 0; i <= steps; ++i) {
            /*
                  /|
                 / |
                /  |
               /   |
              /    |
             /     |
            /______|
            cos 临边:斜边 radius 为斜边, 临边长度为x
            sin 对比:斜边
            */
            float newX = radius * cos(angle * i);
            float newY = radius * sin(angle * i);

            glBegin(GL_TRIANGLES);
            glColor3f(0, (float)i / steps, 0);
            glVertex3f(0.f, 0.f, 0.f);
            glVertex3f(prevX, prevY, 0);
            glVertex3f(newX, newY, 0);
            glEnd();

            prevX = newX;
            prevY = newY;
        }

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }

    glfwTerminate();

    cout << "program closed" << endl;
    return 0;
}
