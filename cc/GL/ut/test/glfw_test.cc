#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/trigonometric.hpp>
#include <iterator>
#include <stdexcept>

#include <iostream>
#include <utility>

#include <GLFW/glfw3.h>

using std::cout, std::endl;

static const float PI = 3.1415926;

static GLfloat W{800}, H{600}; // window size trace

static void onResize(auto w, auto h);
static void onResize(auto window, auto w, auto h);
static void onKeyEvent(auto window, auto key, int x, int y, auto z);
static void onMouseEvent(auto window, int button, int state, int x);
static void onMouseMove(int x, int y);



bool bCull    = true;
bool bDepth   = true;  // Depth test flag
bool bOutline = false; // polygon mode

static float Xrot     = 0;
static float Yrot     = 0;
static float Zrot     = 0;
static float g_Scaled = 1.f;

static int   prevX = 0, prevY = 0;
static bool  bLeftMouseButtonPressed = false;
static float sensitivity             = 0.3f;


static GLFWwindow *window;

void PolygonStipple()
{
    glColor3f(1, 0, 0);
    glEnable(GL_POLYGON_STIPPLE);


    GLubyte fire[] = {
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x92, 0x29, 0x00,
        0x00, 0xb0, 0x48, 0x00,
        0x00, 0xc8, 0x90, 0x00,
        0x00, 0x85, 0x10, 0x00,
        0x00, 0x03, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00};

    glPolygonStipple(fire);

    glScaled(200, 200, 0);

    glBegin(GL_POLYGON);
    {
        glVertex2f(-1, -1);
        glVertex2f(-2, 0);
        glVertex2f(-1, 1);

        glVertex2f(1, 1);
        glVertex2f(2, 0);
        glVertex2f(1, -1);
    }
    glEnd();
}



void Render()
{
    if (bCull)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    if (bDepth) {
        glEnable(GL_DEPTH_TEST);
        // cout << "enabled depth test\n";
    }
    else
        glDisable(GL_DEPTH_TEST);

    if (bOutline)
        glPolygonMode(GL_BACK, GL_LINE);
    else
        glPolygonMode(GL_BACK, GL_FILL);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    {
        glRotatef(Xrot, 1, 0, 0);
        glRotatef(Yrot, 0, 1, 0);
        glRotatef(Zrot, 0, 0, 1);

        glPushMatrix();
        {
            PolygonStipple();
        }
        glPopMatrix();
    }
    glPopMatrix();


    glfwSwapBuffers(window);
    glfwPollEvents();
}

// set render context
void SetupRC()
{
    glClearColor(0, 0, 0, 1);
    glColor3f(0, 1, 0);

    // shae modl 单调着色 of 着色模式
    // choose polygon's last vertex's color to fill this polygon
    glShadeModel(GL_FLAT);

    // gradualy change the color
    // glShadeModel(GL_SMOOTH);

    glFrontFace(GL_CW);

    // OpenGL configuration
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char **argv)
{
    cout << "hello world!!" << endl;

    if (GLFW_TRUE != glfwInit()) {
        throw std::runtime_error("Filed to init glfw");
    }

    window = glfwCreateWindow(W, H, "glfw", nullptr, nullptr);

    glfwMakeContextCurrent(window);


    glfwSetFramebufferSizeCallback(window, onResize);
    glfwSetKeyCallback(window, onKeyEvent);
    glfwSetMouseButtonCallback(window, onMouseEvent);
    // glutMotionFunc(onMouseMove);



    SetupRC();
    while (!glfwWindowShouldClose(window)) {
        Render();
    }
    glfwTerminate();



    return 0;
}



static void onResize(auto window, auto w, auto h)
{
    if (h == 0)
        h = 1;
    glViewport(0, 0, w, h);
    W = w;
    H = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Keep the shape as origin not tranfom by apsect( W: H )
    GLfloat aspect_ratio = w / (GLfloat)h;
    cout << "W : H |" << aspect_ratio << "\n";

    auto X = std::min(W, H);

    if (w <= h)
        glOrtho(-X / 2.f, X / 2.f,
                -X / 2.f / aspect_ratio, X / 2.f / aspect_ratio,
                X, -X);
    else
        glOrtho(-X / 2.f * aspect_ratio, X / 2.f * aspect_ratio,
                -X / 2.f, X / 2.f,
                X, -X);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


static void onKeyEvent(auto window, auto key, int x, int y, auto z)
{
    std::cout << "Key: " << key << ", x: " << x << ", y: " << y << std::endl;


    // Exit the program if the 'q' key is pressed
    if (key == 'q') {
        exit(0);
    }
    if (key == 'c') {
        bCull = !bCull;
    }

    switch (key) {
    case 'x':
        Xrot += 10.f;
        break;
    case 'y':
        Yrot += 10.f;
        break;
    case 'z':
        Zrot += 10.f;
        break;
    case 'd':
        Xrot = Yrot = Zrot = 0.f;
        break;
    case 'o':
        bOutline = !bOutline;
        break;
    default:
        break;
    }
}

void onMouseEvent(auto window, int button, int state, int x)
{
}

void onMouseMove(int x, int y)
{
    if (bLeftMouseButtonPressed) {
        int dX = x - prevX;
        int dY = y - prevY;
        prevX  = x;
        prevY  = y;

        Xrot += dY * sensitivity;
        Yrot += dX * sensitivity;
    }
}
