
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include <cstddef>
#include <cstdlib>
#include <iostream>

#include <math.h>
#include <stdexcept>

using std::cout, std::endl;

const GLchar *vertex120 = R"END(
#version 120
attribute vec3 inPosition;
void main()
{
	gl_Position = vec4(inPosition, 1);
}
)END";

const GLchar *raster120 = R"END(
#version 120
uniform vec2 res; // resolution
uniform float  time;
void main()
{
	 //   if(gl_FragCoord.y > 780 || gl_FragCoord.y < 20){
	 //   	gl_FragColor = vec4(1, 0 ,0 ,1);
	 //   }else {
	 //		float i = 1.f - (gl_FragCoord.y/ res.y) / 2.f ; // y/600 --> 0 --> 1
	 //		gl_FragColor=vec4(i,i,i,1);
	 //	  }
	 

	vec2 centerPoint = (res- vec2(300,300))/2.f ;
	vec2 currentPoint = gl_FragCoord.xy /2.f;

	if( length(currentPoint - centerPoint)< 100.f ){
		gl_FragColor = vec4 (1,1,1,1);
	}else {
		gl_FragColor = vec4( 0,0,0,1 );
	}


	//	float i = 1.f - (gl_FragCoord.y / res.y)/2.f;
	//	gl_FragColor = vec4( i* abs(i* sin(time)),
	//						abs(sin(i* time* 2.f)) , 
	//						i* abs(sin(i* time/3.f)) ,
	//						1.f );
	
}
)END";

int main()
{
    std::cout << "hello world" << std::endl;



    // --------------------------- init
    GLFWwindow *p_window;

    if (!glfwInit()) {
        throw std::runtime_error("glfw init error!");
    }

    const GLubyte *bytes = glGetString(GL_VERSION);
    std::cout << "GL verison: " << bytes << endl;

    p_window = glfwCreateWindow(800, 800, "Hello GLFW", 0, 0);
    if (!p_window) {
        throw std::runtime_error("create window error");
    }

    glfwMakeContextCurrent(p_window);

    // -------------------------------------- shader and program

    GLuint vertexShader;
    GLint  compilationStatus;
    {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertex120, 0);
        glCompileShader(vertexShader);

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationStatus);
        if (compilationStatus == GL_FALSE) {
            GLchar message[256];
            glGetShaderInfoLog(vertexShader, sizeof(message), 0, message);
            cout << "erro when compiling vertex-shader: " << message << endl;
            exit(-1);
        }
    }
    GLuint fragmentShader;
    {
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &raster120, 0);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationStatus);
        if (compilationStatus == GL_FALSE) {
            GLchar message[256];
            glGetShaderInfoLog(vertexShader, sizeof(message), 0, &message[0]);
            cout << "erro when compiling-fragmentShader: " << message << endl;
            exit(-1);
        }
    }


    // shader program
    GLuint shaderProgram;
    {
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetShaderiv(shaderProgram, GL_LINK_STATUS, &compilationStatus);
        if (compilationStatus == GL_FALSE) {
            GLchar message[256];
            glGetProgramInfoLog(shaderProgram, sizeof(message), 0, &message[0]);
            cout << "erro when linking program: " << message << endl;
            exit(-1);
        }
    }

    // aply program
    glUseProgram(shaderProgram);



    // --------------------------------------------VBOs;
    GLfloat positions[] =
        {-1, -1, 0,
         -1, 1, 0,
         1, -1, 0,
         1, -1, 0,
         -1, 1, 0,
         1, 1, 0};

    GLuint positionData;
    glGenBuffers(1, &positionData);
    glBindBuffer(GL_ARRAY_BUFFER, positionData);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW); // memcpy() to GPU's buf


    // ------------------------- Attrib
    GLuint attribPos;

    attribPos = glGetAttribLocation(shaderProgram, "inPosition");
    glEnableVertexAttribArray(attribPos);
    glBindBuffer(GL_ARRAY_BUFFER, positionData);
    glVertexAttribPointer(attribPos, 3, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint uniformRes = glGetUniformLocation(shaderProgram, "res");
    glUniform2f(uniformRes, 600., 600.);


    GLuint uniformTime = glGetUniformLocation(shaderProgram, "time");

    // ------------------------------- VAO



    // 4. render loop
    while (!glfwWindowShouldClose(p_window))
    {

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        glUniform1f(uniformTime, time);


        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(p_window);
        glfwPollEvents();
    }

    glfwTerminate();

    cout << "program closed" << endl;
    return 0;
}
