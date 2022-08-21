#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#define WIN_W 500
#define WIN_H 500

#define VERT_SHADER_FILE "shader.vert"
#define FRAG_SHADER_FILE "shader.frag"

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
    const GLchar* vertShaderSource = vertShaderFileContent.data();

    // read frag shader
    ifstream fragShaderFile (FRAG_SHADER_FILE);
    string fragShaderFileContent;
    while (fragShaderFile) {
        getline(fragShaderFile, fBuf);
        fragShaderFileContent.append(fBuf+'\n');
    }
    fragShaderFile.close();
    const GLchar* fragShaderSource = fragShaderFileContent.data();

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



    // define verticies of a triangle
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    

    // create VAO
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    // bind and configure this VAO
    glBindVertexArray(VAO);

        // setup a VBO from which vertex shader can take attributes from
        GLuint VBO;
        glGenBuffers(1, &VBO);
        // bind and configure this VBO, set(+alloc) vertex attributes
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            // specify where to get vertex attribute data from currently bound VBO
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // no more changes to VBO, so unbind it

        glBindBuffer(GL_ARRAY_BUFFER, 0);

    // no more changes to VAO, so unbind it
    glBindVertexArray(0);
    


    // compile shader program
    GLuint shaderProgram = compileShaders();


    while (!glfwWindowShouldClose(window)) {
        // rendering
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
        handleKeyInput(window);
        
        glClearColor(0.09f, 0.07f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

    }

    glfwTerminate();

    return 0;
}