#include <GL/freeglut_std.h>
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
#include <ut.h>

#include <iostream>
#include <utility>
using std::cout, std::endl;

static const float PI = 3.1415926;

static GLfloat W{800}, H{600}; // window size trace

static void onResize(auto w, auto h);
static void onKeyEvent(unsigned char key, int x, int y);
static void onResize(auto w, auto h);
static void onKeyEvent(unsigned char key, int x, int y);
static void onMouseEvent(int button, int state, int x, int y);
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
    glTranslated(0.5f, 0, 0);

    glBegin(GL_POLYGON);
    {
        glVertex2f(-1, -1);
        glVertex2f(-2, 0);
        glVertex2f(-1, 1);

        glVertex2f(0, 1);
        glVertex2f(1, 0);
        glVertex2f(0, -1);
    }
    glEnd();
}

void Scissor()
{
    // blue
    glClearColor(0.f, 0.f, 1.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    {
        // red
        glClearColor(1, 0, 0, 0);
        glScissor(100, 100, 600, 400);
        glClear(GL_COLOR_BUFFER_BIT);

        // green
        glClearColor(0, 1, 0, 0);
        glScissor(200, 200, 400, 200);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    glDisable(GL_SCISSOR_TEST);
}


// mask, 模板， 漏字板
void Stencil()
{

    float r = 0.1f;
    float angle;

    glClearColor(0, 0, 1, 0);

    // 0 represent `clear stencil`
    glClearStencil(0.f);
    glEnable(GL_STENCIL_TEST);
    {
        glClear(GL_STENCIL_BUFFER_BIT);

        // Render all into STENCIL buffer (NEVER & INCR)
        glStencilFunc(GL_NEVER, 0x0, 0x0);      // never pass if not cross STENCIL test
        glStencilOp(GL_INCR, GL_INCR, GL_INCR); // and let not pass add to value in STENCIL test buffer

        glColor3f(1, 1, 1);
        glBegin(GL_LINE_STRIP);
        {
            for (angle = 0; angle < 400.f; angle += 0.1) {
                glVertex2d(r * std::cos(angle), r * std::sin(angle));
                r *= 1.002;
            }
        }
        glEnd();

        // Now, allowed draw, but except value eq 0x01 with STENCIL, and did not modify STENCIL buffer
        glStencilFunc(GL_NOTEQUAL, 0x1, 0x1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

        glColor3f(1.f, 0.f, 0.f);
        glRectf(x, y, x + 50, r - 50);
    }
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
            // Scissor();
        }
        glPopMatrix();
    }
    glPopMatrix();


    glutSwapBuffers();
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
}

int main(int argc, char **argv)
{
    cout << "hello world!!" << endl;

    glutInit(&argc, argv);
    glutInitWindowSize(W, H);
    glutCreateWindow("Triangle");

    // double buffer, ... , ..., 模板缓冲
    // Can request the buffer dynamicaly
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

    glutReshapeFunc(onResize);
    glutKeyboardFunc(onKeyEvent);
    glutMouseFunc(onMouseEvent);
    glutMotionFunc(onMouseMove);

    glutDisplayFunc(Render);

    SetupRC();

    glutMainLoop();

    return 0;
}



static void onResize(auto w, auto h)
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


static void onKeyEvent(unsigned char key, int x, int y)
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

    glutPostRedisplay();
}

void onMouseEvent(int button, int state, int x, int y)
{
    printf("{x: %d, y: %d}\n", x, y);

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            bLeftMouseButtonPressed = true;
            prevX                   = x;
            prevY                   = y;
        }
        else if (state == GLUT_UP)
        {
            bLeftMouseButtonPressed = false;
            prevX                   = W / 2;
            prevY                   = H / 2;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // static int SettingWindow;
        // glutCreateSubWindow(glutGetWindow(), x, y, W / 3, H / 3);
        // glutDisplayFunc(Render);
        // Right mouse button pressed
        // Handle the event here
    }
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        // Middle mouse button pressed
        // Handle the event here
    }
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

        glutPostRedisplay();
    }
}
