
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <corecrt.h>
#include <iostream>
#include <stdexcept>

using std::cout, std::endl;

const int steps = 10;
// 2 pie == 360
const float angle = 3.1415926 * 2.f / steps;

int main() {
  std::cout << "hello world" << std::endl;

  GLFWwindow *p_window;

  // 1. init
  if (!glfwInit()) {
    throw std::runtime_error("glfw init error!");
  }

  // 2. create window
  p_window = glfwCreateWindow(800, 600, "Hello GLFW", 0, 0);
  if (!p_window) {
    throw std::runtime_error("create window error");
  }

  // 3. bind window context
  glfwMakeContextCurrent(p_window);

  float xPos = 0;
  float yPos = 0;
  float radius = 1.0f;

  // 4. main loop
  while (!glfwWindowShouldClose(p_window)) {

    glClearColor(0, 0, 1, 0);

    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);

    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.6f, 0);

    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.6f, 0);

    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.0f, 0.6f, 0);

    glEnd();

    glfwSwapBuffers(p_window);
    glfwPollEvents();
  }

  glfwTerminate();

  cout << "program closed" << endl;
  return 0;
}
