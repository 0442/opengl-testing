#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

#define VERT_SHADER_FILE "shader.vert"
#define FRAG_SHADER_FILE "shader.frag"

const uint WIN_W = 500;
const uint WIN_H = 500;

GLuint compileShaders() {
    using namespace std;
    
    string fBuf;

    // read shader files into strings
    // read vert shader
    ifstream vertShaderFile (VERT_SHADER_FILE);
    string vertShaderFileContent;
    while (vertShaderFile) {
        getline(vertShaderFile, fBuf);
        vertShaderFileContent.append(fBuf+'\n');
    }
    vertShaderFile.close();
    const GLchar* vertShaderSource = vertShaderFileContent.c_str();

    // read frag shader
    ifstream fragShaderFile (FRAG_SHADER_FILE);
    string fragShaderFileContent;
    while (fragShaderFile) {
        getline(fragShaderFile, fBuf);
        fragShaderFileContent.append(fBuf+'\n');
    }
    fragShaderFile.close();
    const GLchar* fragShaderSource = fragShaderFileContent.c_str();

    // create and compile shaders
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertShaderSource, NULL);
    glCompileShader(vertShader);

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, NULL);
    glCompileShader(fragShader);

    // shader compilation error checking 
    int successStatus;
    char shaderInfoLog[512];
    // for vertex shader
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &successStatus);
    if (!successStatus) {
        glGetShaderInfoLog(vertShader, 512, NULL, shaderInfoLog);
        cout << "error: vertex shader compilation failed: " << shaderInfoLog << endl;
    }
    // same for the fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &successStatus);
    if (!successStatus) {
        glGetShaderInfoLog(fragShader, 512, NULL, shaderInfoLog);
        cout << "error: fragment shader compilation failed: " << shaderInfoLog << endl;
    }


    cout << vertShaderSource << endl << fragShaderSource << endl;

    // create shader program and attach shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    // shader program error checking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successStatus);
    if (!successStatus) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, shaderInfoLog);
        cout << "error: program linking failed: " << shaderInfoLog << endl;
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return shaderProgram;
}

GLFWwindow* windowSetup() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIN_W, WIN_H, "OpenGL testing", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create window." << std::endl;
        glfwTerminate();
    }

    return window;
}

void handleKeyInput(GLFWwindow* window) {
    // quit when Q is pressed
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } 
}

int main() {
    // create a glfw window
    GLFWwindow* window = windowSetup();
    glfwMakeContextCurrent(window);

    // make and opengl viewport
    glViewport(0, 0, WIN_W, WIN_H);

    // compile shader program
    GLuint shaderProgram = compileShaders();



    // verticies of a square
    GLfloat vertices[] = {
        //   positions             colors
         0.5f,  0.5f, 0.0f,   1.0f, 0.8f, 0.4f,
         0.5f, -0.5f, 0.0f,   1.0f, 0.7f, 0.5f,
        -0.5f, -0.5f, 0.0f,   1.0f, 0.6f, 0.6f,
        -0.5f,  0.5f, 0.0f,   1.0f, 0.7f, 0.5f    
    };

    // indices for EBO to draw a square
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    

    // create VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    // bind and configure this VAO
    glBindVertexArray(VAO);

        // EBO has indices for element drawing
        
        GLuint EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        

        // setup a VBO from which vertex shader can take attributes from
        GLuint VBO;
        glGenBuffers(1, &VBO);
        // bind and configure this VBO ( set(+alloc) vertex attributes)
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // specify how buffer is constructed / where to get different vertex attribute data from
            // positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // colors
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

    // no more changes to VAO, so unbind all
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // get uniform locations
    GLint fragTimeUnifLocation = glGetUniformLocation(shaderProgram, "changingColor");
    GLint xOffsetLocation = glGetUniformLocation(shaderProgram, "xOffset");
    GLint yOffsetLocation = glGetUniformLocation(shaderProgram, "yOffset");



    while (!glfwWindowShouldClose(window)) {
        // rendering
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // update uniforms
        float currTime = glfwGetTime();
        float changingNum = sin(int(currTime*100) % 1000 / 1000.0f * 3.141592f );
        glUniform4f(fragTimeUnifLocation, 0.6f, 0.8f, changingNum*1.0, 1.0f);

        glUniform1f(xOffsetLocation, cos(currTime*8)/2);
        glUniform1f(yOffsetLocation, sin(currTime*8)/2);


        glfwSwapBuffers(window);
        glfwPollEvents();
        handleKeyInput(window);
        
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

    }

    glfwTerminate();

    return 0;
}