#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <cmath>
#include <cstdio>
#include <ctime>
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

    // create cull area (l, r, bot, top close, far, )
    GLfloat aspect_ratio = w / (GLfloat)h;
    cout << "W : H |" << aspect_ratio << "\n";

    if (w <= h)
        glOrtho(-XSize, XSize,
                -YSize / aspect_ratio, YSize / aspect_ratio,
                ZSize, -ZSize);
    else
        glOrtho(-XSize * aspect_ratio, XSize * aspect_ratio,
                -YSize, YSize,
                ZSize, -ZSize);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void onKeyEvent(unsigned char key, int a, int b)
{
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
    glutDisplayFunc(IncreasingPointCircle);
    glutDisplayFunc(Lines);

    SetupRC();

    glutMainLoop();

    return 0;
}
