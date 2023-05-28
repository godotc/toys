#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <cstdio>
#include <ut.h>

#include <iostream>
using std::cout, std::endl;

GLfloat x1{0.}, y1{0.}, rsize{75.}, // location and size
    xstep{10.}, ystep{10.},         // every  stride
    W{800}, H{600};                 // window size trace


void RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 0, 0);

    glRectf(x1, y1, x1 + rsize, y1 - rsize);

    // flush and swap
    glutSwapBuffers();
}

// on window havn't change or move (leisure)
void TimerFunc(int value)
{
    if (x1 > W - rsize || x1 < -W) {
        xstep = -xstep;
    }
    if (y1 > H || y1 < -H + rsize) {
        ystep = -ystep;
    }

    x1 += xstep;
    y1 += ystep;

    // check boundary, avoid window loc outsise cull on resing

    if (x1 > (W + xstep - rsize)) {
        x1 = W - rsize - 1;
    }
    else if (x1 < -W + xstep) {
        x1 = -W + 1;
    }

    if (y1 > (H + ystep)) {
        y1 = H - 1;
    }
    else if (y1 < -(H - rsize + ystep)) {
        y1 = -H + rsize - 1;
    }



    printf("LT: {%f,  %f}\n", x1, y1);

    // redraw and set next timer
    glutPostRedisplay();
    glutTimerFunc(33, TimerFunc, 1);
}

// set render condition
void SetupRC()
{
    glClearColor(0.3, 0.4, 0.5, 1.0);
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
        glOrtho(-W, W,
                -H / aspect_ratio, H / aspect_ratio,
                1.0, -1.0);
    else
        glOrtho(-W * aspect_ratio, W * aspect_ratio,
                -H, H,
                1.0, -1.0);

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
    glutCreateWindow("GLUT");

    glutDisplayFunc(RenderScene);
    glutReshapeFunc(onResize);
    glutTimerFunc(33, TimerFunc, 1);
    glutKeyboardFunc(onKeyEvent);

    SetupRC();

    glutMainLoop();

    return 0;
}
