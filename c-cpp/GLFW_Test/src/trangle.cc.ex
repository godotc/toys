
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <math.h>
#include <stdexcept>

using std::cout, std::endl;



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


    // 4. render loop
    while (!glfwWindowShouldClose(p_window)) {

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

		glColor3f(1,0,0);
		glVertex3f(-1,-1,0);
		glColor3f(0,1,0);
		glVertex3f(1,-1,0);
		glColor3f(0,0,1);
		glVertex3f(1,1,0);


        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }

    glfwTerminate();

    cout << "program closed" << endl;
    return 0;
}
