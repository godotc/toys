#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <ut.h>

#include <iostream>
using std::cout, std::endl;



void Rect()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0, 0, 0);
    glRectf(-25, -25, 25, 25);

    glFlush();
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // create cull area (l, r, bot, top close, far, )
    GLfloat aspect_ratio = w / (GLfloat)h;
    cout << "W : H |" << aspect_ratio << "\n";
    if (w <= h)
        glOrtho(-100, 100,
                -100 / aspect_ratio, 100 / aspect_ratio,
                1.0, -1.0);
    else
        glOrtho(-100 * aspect_ratio, 100 * aspect_ratio,
                -100, 100,
                1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char **argv)
{
    cout << "hello world!" << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

    glutInitWindowSize(800, 600);

    glutCreateWindow("GLUT");
    glutDisplayFunc(Rect);
    glutReshapeFunc(onResize);

    SetupRC();


    glutMainLoop();


    return 0;
}
