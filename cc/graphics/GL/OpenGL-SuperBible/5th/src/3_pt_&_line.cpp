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

GLint        t     = 100;
static GLint XSize = t, YSize = t;
static GLint ZSize = t;


void PointCircle()
{
    float size = 100;

    glClear(GL_COLOR_BUFFER_BIT);

    // Save current matrix
    glPushMatrix();


    float xRot = -70;
    float yRot = -25;

    glRotatef(xRot, 1.0, 0, 0);
    glRotatef(yRot, 0, 1, 0);
    // glRotatef(yRot, 20, 0, 1);
    glTranslated(0, 0, -size * 2);

    glBegin(GL_POINTS);


    static float z      = -size;
    const float  rounds = 7.f;

    GLfloat x, y, angle;
    for (angle = 0.f; angle < 2 * PI * rounds; angle += 0.1f) {
        x = size * sin(angle);
        y = size * cos(angle);

        glVertex3f(x, y, z);

        z += 0.5f;
    }

    glEnd();


    // Restore the stored view matrix (back to the previous matrix view port or some shit)
    glPopMatrix();

    // flush and swap
    glutSwapBuffers();
}
void IncreasingPointCircle()
{
    static GLfloat ptSize;
    GLfloat        ptStride;
    {
        GLfloat ptSizeRange[2];
        glGetFloatv(GL_POINT_SIZE, ptSizeRange);
        glGetFloatv(GL_POINT_SIZE_GRANULARITY, &ptStride);
        ptSize = ptSizeRange[0];
    }

    glClear(GL_COLOR_BUFFER_BIT);


    // Save current matrix
    glPushMatrix();
    {
        float size = t / 4;

        float xRot = -70;
        float yRot = -25;

        glRotatef(xRot, 1.0, 0, 0);
        glRotatef(yRot, 0, 1, 0);
        // glRotatef(yRot, 20, 0, 1);
        glTranslated(0, 0, -size * 2);



        static float z      = -size;
        const float  rounds = 7.f;

        GLfloat x, y, angle;
        for (angle = 0.f; angle < 2 * PI * rounds; angle += 0.1f) {
            x = size * sin(angle);
            y = size * cos(angle);

            // update size before `glBegin`
            glPointSize(ptSize);

            glBegin(GL_POINTS);
            {
                glVertex3f(x, y, z);
            }
            glEnd();

            ptSize += ptStride;
            // cout << "Size" << ptSize << '\n';
            z += 0.5f;
        }
    }
    // Restore the stored view matrix (back to the previous matrix view port or some shit)
    glPopMatrix();

    // flush and swap
    glutSwapBuffers();
}

void Lines()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float angle;
    float r = t / 2;
    float z = 0.f;
    // glLineWidth(20.f);
    glBegin(GL_LINES);
    {
        glVertex3f(0, 0, 0);
        glVertex3f(100, 100, 0);
        float x, y;
        for (angle = 0.f; angle < PI; angle += (PI / 20.f)) {
            // upper
            x = r * std::cos(angle);
            y = r * std::sin(angle);

            glVertex3f(x, y, z);

            // bottom
            x = r * std::cos(angle + PI);
            y = r * std::sin(angle + PI);
            glVertex3f(x, y, z);
        }
    }
    glEnd();

    glutSwapBuffers();
}

void StripLine()
{
    glClear(GL_COLOR_BUFFER_BIT);

#if Strip
    glBegin(GL_LINE_STRIP);
#else // loop
    glBegin(GL_LINE_LOOP);
#endif
    {
        glVertex3f(0., 0., 0.);
        glVertex3f(50., 50., 0.);
        glVertex3f(50., 100., 0.);
    }
    glEnd();

    glutSwapBuffers();
}

void StripLinesCircle()
{
    GLfloat line_width_range[2];
    GLfloat steps;

    glGetFloatv(GL_LINE_WIDTH_RANGE, line_width_range);
    glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &steps);



    auto draw = []() {
#if 1
        float width_range[2];
        float step;
        glGetFloatv(GL_LINE_WIDTH_RANGE, width_range);
        glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &step);

        float current_width = width_range[0];

        float z      = -50.f;
        float rounds = 7.f;
        float size   = 120.f;
        float x, y;
        for (float angle = 0.f; angle < PI * 2 * rounds; angle += 0.2f)
        {

            glLineWidth(current_width);
            current_width += step;

            glBegin(GL_LINE_STRIP);
            {
                x = size * sin(angle);
                y = size * cos(angle);
                glVertex3f(x, y, z);
                x = size * sin(angle + 0.1f);
                y = size * cos(angle + 0.1f);
                glVertex3f(x, y, z);
            }
            glEnd();

            z += 0.5f;
        }
#else
        glBegin(GL_LINE_STRIP);
        {

            float z      = -50.f;
            float rounds = 7.f;
            float size   = 120.f;
            float x, y;
            for (float angle = 0.f; angle < PI * 2 * rounds; angle += 0.1f)
            {
                x = size * sin(angle);
                y = size * cos(angle);
                glVertex3f(x, y, z);
                z += 0.5f;
            }
        };
        glEnd();
#endif
    };

    glPushMatrix();
    {
        glTranslated(-250, 0, 0);
        glRotatef(45.f, 0, 1, 0);
        glRotatef(45, 1, 0, 0);
        draw();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslated(250, 0, 0);
        glRotatef(45, 1, 0, 0);
        glRotatef(45.f, 0, 1, 0);
        draw();
    }
    glPopMatrix();
}

// @dev 点画线:
void Stippingling()
{
    glEnable(GL_LINE_STIPPLE);

    GLint    factor  = 1;      // 乘法因子
    GLushort pattern = 0x5555; // 点画模式

    glLineStipple(factor, pattern);
}


void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    StripLinesCircle();

    glutSwapBuffers();
}

// set render condition
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
                ZSize, -ZSize);
    else
        glOrtho(-X / 2.f * aspect_ratio, X / 2.f * aspect_ratio,
                -X / 2.f, X / 2.f,
                ZSize, -ZSize);

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


int main(int argc, char **argv)
{
    cout << "hello world!!" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(W, H);
    glutCreateWindow("Circle");

    glutReshapeFunc(onResize);
    glutKeyboardFunc(onKeyEvent);

    // glutDisplayFunc(PointCircle);
    // glutDisplayFunc(IncreasingPointCircle);
    // glutDisplayFunc(Lines);
    // glutDisplayFunc(StripLine);

    glutDisplayFunc(Render);

    SetupRC();

    glutMainLoop();

    return 0;
}
