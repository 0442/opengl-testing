#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include "implement_stb_image.cpp"

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

    // make an opengl viewport
    glViewport(0, 0, WIN_W, WIN_H);

    // compile shader program
    GLuint shaderProgram = compileShaders();

    // gl enables
    glEnable(GL_DEPTH_TEST);


    // verticies of a square

    GLfloat vertices[] = {
        //vert. positions          colors         tex. positions
        // top
         0.5f,  0.5f,  0.5f,   1.0f, 0.8f, 0.4f,    1.0f,  1.0f, 
         0.5f, -0.5f,  0.5f,   1.0f, 0.7f, 0.5f,    1.0f,  0.0f, 
        -0.5f, -0.5f,  0.5f,   1.0f, 0.6f, 0.6f,    0.0f,  0.0f, 
        -0.5f,  0.5f,  0.5f,   1.0f, 0.7f, 0.5f,    0.0f,  1.0f,
        // bottom
         0.5f,  0.5f, -0.5f,   1.0f, 0.8f, 0.4f,    1.0f,  1.0f, 
         0.5f, -0.5f, -0.5f,   1.0f, 0.7f, 0.5f,    1.0f,  0.0f, 
        -0.5f, -0.5f, -0.5f,   1.0f, 0.6f, 0.6f,    0.0f,  0.0f, 
        -0.5f,  0.5f, -0.5f,   1.0f, 0.7f, 0.5f,    0.0f,  1.0f,
        // front
         0.5f,  0.5f,  0.0f,   1.0f, 0.8f, 0.4f,    1.0f,  1.0f, 
         0.5f, -0.5f,  0.0f,   1.0f, 0.7f, 0.5f,    1.0f,  0.0f, 
        -0.5f, -0.5f,  0.0f,   1.0f, 0.6f, 0.6f,    0.0f,  0.0f, 
        -0.5f,  0.5f,  0.0f,   1.0f, 0.7f, 0.5f,    0.0f,  1.0f,
        // back
         0.5f,  0.5f,  0.0f,   1.0f, 0.8f, 0.4f,    1.0f,  1.0f, 
         0.5f, -0.5f,  0.0f,   1.0f, 0.7f, 0.5f,    1.0f,  0.0f, 
        -0.5f, -0.5f,  0.0f,   1.0f, 0.6f, 0.6f,    0.0f,  0.0f, 
        -0.5f,  0.5f,  0.0f,   1.0f, 0.7f, 0.5f,    0.0f,  1.0f,
        // left
         0.5f,  0.5f,  0.0f,   1.0f, 0.8f, 0.4f,    1.0f,  1.0f, 
         0.5f, -0.5f,  0.0f,   1.0f, 0.7f, 0.5f,    1.0f,  0.0f, 
        -0.5f, -0.5f,  0.0f,   1.0f, 0.6f, 0.6f,    0.0f,  0.0f, 
        -0.5f,  0.5f,  0.0f,   1.0f, 0.7f, 0.5f,    0.0f,  1.0f,
        // right
         0.5f,  0.5f,  0.0f,   1.0f, 0.8f, 0.4f,    1.0f,  1.0f, 
         0.5f, -0.5f,  0.0f,   1.0f, 0.7f, 0.5f,    1.0f,  0.0f, 
        -0.5f, -0.5f,  0.0f,   1.0f, 0.6f, 0.6f,    0.0f,  0.0f, 
        -0.5f,  0.5f,  0.0f,   1.0f, 0.7f, 0.5f,    0.0f,  1.0f,
    };

    // indices for EBO to draw a square
    GLuint indices[] = {
        //top
        0, 1, 3,
        1, 2, 3,
        //bottom
        4, 5, 6,
        5, 6, 7,
    };



    // texturing
    // tex params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load texture
    int imgX, imgY, imgComp; 
    unsigned char* imgData = stbi_load("texture.jpeg", &imgX, &imgY, &imgComp, 0);
    if (!imgData) {
        std::cout << "Failed to load textures" << std::endl;
    }

    // generate 
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgX, imgY, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
    glGenerateMipmap(GL_TEXTURE_2D);
    // no more changes, unbind and free image memory
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(imgData);
    


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
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // colors
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            // texture positions
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6*sizeof(float)));
            glEnableVertexAttribArray(2);

    // no more changes to VAO, unbind all
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);



    // get uniform locations
    GLint changingNumLocation = glGetUniformLocation(shaderProgram, "changingNum");
    GLint xOffsetLocation = glGetUniformLocation(shaderProgram, "xOffset");
    GLint yOffsetLocation = glGetUniformLocation(shaderProgram, "yOffset");
    GLint textureLocation = glGetUniformLocation(shaderProgram, "texture1");
    // transformation matricies
    GLint modelMatLocation = glGetUniformLocation(shaderProgram, "model");
    GLint cameraMatLocation = glGetUniformLocation(shaderProgram, "camera");
    GLint projectionMatLocation = glGetUniformLocation(shaderProgram, "projection");



    while (!glfwWindowShouldClose(window)) {
        float currTime = glfwGetTime();
        float changingNum = sin(((int(currTime*1000) % 10000) / 10000.0f) * 3.141592f );

        // matricies
        //model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        //lookAt matrix
        glm::vec3 cameraPos = glm::vec3(cos(currTime), sin(currTime), 1.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraZvec= glm::normalize(cameraPos - cameraTarget);
        glm::vec3 worldYvec = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraXvec = glm::normalize(glm::cross(worldYvec, cameraZvec));
        glm::vec3 cameraYvec = glm::cross(cameraZvec, cameraXvec);

        glm::mat4 lookAt = glm::lookAt(cameraPos, cameraTarget, cameraYvec);

        //projection
        glm::mat4 projection = glm::mat3(1.0f);
        projection = glm::perspective(glm::radians(75.0f), (1.0f * WIN_W) / WIN_H, 0.1f, 100.0f);


        // rendering
        glUseProgram(shaderProgram);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // update uniforms
        glUniform1f(changingNumLocation, changingNum);

        glUniform1f(xOffsetLocation, 0.0f);
        glUniform1f(yOffsetLocation, 0.0f);

        glUniform1i(textureLocation, 0);

        glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(cameraMatLocation, 1, GL_FALSE, glm::value_ptr(lookAt));
        glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, glm::value_ptr(projection));


        glfwSwapBuffers(window);
        glfwPollEvents();
        handleKeyInput(window);
        
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    glfwTerminate();

    return 0;
}