import sys
from OpenGL.GLUT import *
from OpenGL.GL import *


def dispaly():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glColor3f(1, 1, 0)
    glRectf(-0.5, 0.5, 0.5, -0.5)
    glFlush()


glutInit(sys.argv)
glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB)
glutInitWindowSize(400, 400)
glutCreateWindow("oldgl")
glutDisplayFunc(dispaly)
glutMainLoop()
