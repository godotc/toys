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
#include <math.h>
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

static float Xrot = 0;
static float Yrot = 0;
static float Zrot = 0;

static int   prevX = 0, prevY = 0;
static bool  bLeftMouseButtonPressed = false;
static float sensitivity             = 0.3f;

// 圆锥
void CicularCone()
{
    glRotatef(Xrot, 1, 0, 0);
    glRotatef(Yrot, 0, 1, 0);
    glRotatef(Zrot, 0, 0, 1);


    float      r = 100.f;
    float      x, y;
    static int iPivot = 0.f; // flag for color switch between red & green
    float      step   = 21;
    step /= 2.f;

    // conicoid 圆锥曲面
    glBegin(GL_TRIANGLE_FAN);
    {
        glVertex3f(0.f, 0.f, -300);

        for (float angle = 0.f; angle <= PI * 2; angle += PI / step) {
            x = r * cos(angle);
            y = r * sin(angle);

            if (iPivot % 2 == 0)
                glColor3f(0, 1, 0);
            else
                glColor3f(1, 0, 0);
            iPivot++;

            // x,y,0
            glVertex2f(x, y);
        };
    }
    glEnd();

    // bottom of cicular cone: Circle
    glBegin(GL_TRIANGLE_FAN);
    {
        glVertex2f(0, 0); // origin

        for (float angle = 0.f; angle <= PI * 2; angle += PI / step) {
            x = r * sin(angle);
            y = r * cos(angle);

            if (iPivot % 2 == 0)
                glColor3f(0, 1, 0);
            else
                glColor3f(1, 0, 0);
            iPivot++;

            // x,y,0
            glVertex2f(x, y);
        };
    }
    glEnd();
}


void CW_CCW()
{
    auto getXY = [](auto r, auto i, bool bInverse) -> std::pair<float, float> {
        float x, y;
        // the direction as angle(i)'s trend fomr 0 ~ PI , so it's the anti clockwise
        if (bInverse) {
            x = r * cos(i); //    /|
            y = r * sin(i); //  r/ | r * sin(x) -> Point's y
                            //  /  |
                            // x___|
                            //  r * cos(x) -> Points's x
        }
        // It's postio will from PI/2 -> clock-wise
        else {
            x = r * sin(i);
            y = r * cos(i);
        }
        return {x, y};
    };

    int   bRed = 0;
    float r    = 80.f;
    float step = 30 / 2;

    glPushMatrix();
    {
        glTranslated(-200, 0, 0);
        glBegin(GL_TRIANGLE_FAN);
        {
            glVertex2f(0, 0);

            for (float i = 0; i <= PI * 2; i += PI / step) {
                auto &&[x, y] = getXY(r, i, false);

                if (bRed % 2 == 0)
                    glColor3f(1, 0, 0);
                else
                    glColor3f(0, 1, 0);
                ++bRed;

                glVertex2f(x, y);
            }
        }
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(200, 0, 0);
        glBegin(GL_TRIANGLE_FAN);
        {
            glVertex2f(0, 0);

            for (float i = 0; i <= PI * 2; i += PI / step) {
                auto &&[x, y] = getXY(r, i, true);

                if (bRed % 2 == 0)
                    glColor3f(1, 0, 0);
                else
                    glColor3f(0, 1, 0);
                ++bRed;

                glVertex2f(x, y);
            }
        }
        glEnd();
    }
    glPopMatrix();
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
    CicularCone();
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
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(W, H);
    glutCreateWindow("Triangle");

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
