#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <math.h>
#include <ut.h>

#include <iostream>
using std::cout, std::endl;

static const float PI = 3.1415926;

static GLfloat W{800}, H{600}; // window size trace

static void onResize(auto w, auto h);
static void onKeyEvent(unsigned char key, int x, int y);
void        SetupRC();
static void onResize(auto w, auto h);
static void onKeyEvent(unsigned char key, int x, int y);


void Triangles()
{
    // Set the CW (Clock-Wise) as front facae, CWW on the contarry
    glFrontFace(GL_CW);

    glTranslated(-100, 0, 0);
    glScaled(50, 50, 0);

    glBegin(GL_TRIANGLES);
    {
        glVertex3f(-1, -1, 0);
        glVertex3f(-1, 1, 0);
        glVertex3f(1, 1, 0);

        glVertex3f(-1, -1, 0);
        glVertex3f(1, 1, 0);
        glVertex3f(1, -1, 0);
    }
    glEnd();
}
void TriangleFans()
{
    // Set the CW (Clock-Wise) as front facae, CWW on the contarry
    glFrontFace(GL_CCW);

    glTranslated(100, 0, 0);
    glScaled(50, 50, 0);


    glBegin(GL_TRIANGLE_FAN);
    {
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1, 0);
        glVertex3f(1, 1, 0);

        glVertex3f(1, 0, 0);
        glVertex3f(1, -1, 0);
    }
    glEnd();
}


void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glPushMatrix();
    Triangles();
    glPopMatrix();

    glPushMatrix();
    TriangleFans();
    glPopMatrix();

    glutSwapBuffers();
}


int main(int argc, char **argv)
{
    cout << "hello world!!" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(W, H);
    glutCreateWindow("Triangle");

    glutReshapeFunc(onResize);
    glutKeyboardFunc(onKeyEvent);

    glutDisplayFunc(Render);

    SetupRC();

    glutMainLoop();

    return 0;
}

// set render context
void SetupRC()
{
    glClearColor(0, 0, 0, 1);
    glColor3f(0, 1, 0);
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
}
