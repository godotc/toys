import os
from sys import path

import numpy as np
from glfw.GLFW import *
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLX import *
from OpenGL.GLUT import *
from numpy.lib import math

glutils = __import__("glutils")


class Scene:
    def __init__(self) -> None:
        self.program = glutils.LoadShaders()

        glUseProgram(self.program)

        self.pMatrixUnifrom = glGetUniformLocation(self.program, "uPMatrix")
        self.mvMatrixUnifrom = glGetUniformLocation(self.program, "uMVMatrix")

        self.tex2D = glGetUniformLocation(self.program, "tex2D")
        vertexDta = np.array([-0.5, -0.5, 0.0,
                              0.5, -0.5, 0.0,
                              - 0.5, 0.5, 0.0,
                              0.5, 0.5, 0.0], np.float32)

        self.vao = glGenVertexArrays(1)
        glBindVertexArray(self.vao)

        self.vertexBuffer = glGenBuffers(1)
        glBindBuffer(GL_ARRAY_BUFFER, self.vertexBuffer)
        glBufferData(GL_ARRAY_BUFFER, 4*len(vertexDta),
                     vertexDta, GL_STATIC_DRAW)

        glEnableVertexAttribArray(0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, None)

        glBindVertexArray(0)

    def step(self):
        self.t = (self.t+1) % 360
        glUniform1f(glGetUniformLocation(
            self.program, "uTheta"), math.radians(self.t))

    def render(self, pMat, mvMat):
        glUseProgram(self.program)
        glUniformMatrix4fv(self.pMatrixUnifrom, 1, GL_FALSE, pMat)
        glUniformMatrix4fv(self.mvMatrixUnifrom, 1, GL_FALSE, mvMat)

        glUniform1i()


class RenderWindow:
    def __init__(self, w, h) -> None:
        cwd = os.getcwd()
        glfwInit()
        os.chdir(cwd)

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE)

        self.width, self.height = w, h
        self.aspect = self.width/float(self.height)

        # create window
        self.window = glfwCreateWindow(
            self.width, self.height, "GLFW", None, None)

        # make contenxt current
        glfwMakeContextCurrent(self.window)

        glViewport(0, 0, self.width, self.height)
        glEnable(GL_DEPTH_TEST)
        glClearColor(0.5, 0.4, 0.5, 1)

        # bind event cb
        glfwSetMouseButtonCallback(self.window, self.onMouseButton)
        glfwSetKeyCallback(self.window, self.onKeyboad)
        glfwSetWindowSizeCallback(self.window, self.onResize)

    def onKeyboad(self, window, key, scancode, action, mods):
        print('keyboard', window, key, scancode, action, mods)
        if action == GLFW_PRESS:
            if (key == GLFW_KEY_ESCAPE):
                self.isEnd = True
            else:
                self.scene.showCicle = not self.scene.showCicle

    def onResize(self, window, w, h):
        self.width = w
        self.h = h
        self.aspect = w/float(h)
        glViewport(0, 0, self.width, self.height)

    def onMouseButton(self, key):
        1

    def run(self):
        glfwSetTime(0)
        t = 0.0

        while not glfwWindowShouldClose(self.window) and not self.isEnd:
            current_time = glfwGetTime()
            if current_time-t > 0.1:
                t = current_time

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BITS)

                proj_matrix = gluPerspective(45, self.aspect, 0.1, 100)
                model_view_matrix = gluLookAt([0.0, 0.0, -2.], [0, 0, 0],
                                              [0, 1, 0])

                self.scene.render(proj_matrix, model_view_matrix)
                self.scene.step()

                glfwSwapBuffers(self.window)
                glfwPollEvents()

        glfwTerminate()
