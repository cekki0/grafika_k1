#define _CRT_SECURE_NO_WARNINGS
#define CRES 30 
#define RATIO 1200/600

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>   
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath);

int main(void)
{
    if (!glfwInit())
    {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 1200;
    unsigned int wHeight = 600;
    const char wTitle[] = "alo kuco";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    unsigned int shader = createShader("basic.vert", "basic.frag");
    unsigned int shaderGlisa = createShader("basic.vert", "glisa.frag");
    unsigned int shaderSun = createShader("basic.vert", "sun.frag");
    unsigned int shaderTrunk = createShader("tree.vert", "tree.frag");
    unsigned int shaderLeaves = createShader("basic.vert", "leaves.frag");
    unsigned int shaderImage = createShader("slika.vert", "slika.frag");
    unsigned int shaderDog = createShader("pas.vert", "pas.frag");
    unsigned int shaderSmoke = createShader("smoke.vert", "smoke.frag");
    unsigned int uA = glGetUniformLocation(shaderSun, "uA");
    unsigned int uPosLoc = glGetUniformLocation(shaderDog, "uPos");
    unsigned int uPosLocSmoke = glGetUniformLocation(shaderSmoke, "uPos");
    unsigned int uRotationAngleLoc = glGetUniformLocation(shaderDog, "uRotationAngle");
    unsigned int uColorProgressLoc = glGetUniformLocation(shaderTrunk, "uColorProgress");
    unsigned int VAO[11]; 
    glGenVertexArrays(11, VAO);
    unsigned int VBO[11];
    glGenBuffers(11, VBO);
 

    float vertices[] =
    {
        // Prvi sprat
        -0.5, 0.0, 1.0, 1.0, 0.0, 1.0,
        0.5, 0.0,  1.0, 1.0, 0.0, 1.0,
        -0.5, -0.4, 1.0, 1.0, 0.0, 1.0,
        0.5, -0.4, 1.0, 1.0, 0.0, 1.0,

        // Prozori na prvom spratu
        -0.35, -0.2, 0.0, 0.0, 1.0, 0.4,
        -0.25, -0.2,  0.0, 0.0, 1.0, 0.4,
        -0.35, 0.2,  0.0, 0.0, 1.0, 0.4,
        -0.25, 0.2,   0.0, 0.0, 1.0, 0.4,

        0.25, -0.2, 0, 0, 1, 0.4,
        0.35, -0.2,  0, 0, 1, 0.4,
        0.25, 0.2,  0, 0, 1, 0.4,
        0.35, 0.2,   0, 0, 1, 0.4,

        // Drugi sprat
        -0.5, 0.0, 1, 1, 0, 1.0,
        0.5, 0.0,  1, 1, 0, 1.0,
        -0.5, 0.4,  1, 1, 0, 1.0,
        0.5, 0.4,   1, 1, 0, 1.0,

        // Prozori na drugom spratu
        -0.35, 0.1, 0, 0, 1, 0.4,
        -0.25, 0.1,  0, 0, 1, 0.4,
        -0.35, 0.3,  0, 0, 1, 0.4,
        -0.25, 0.3,   0, 0, 1, 0.4,

        // Sivi odžak
        0.25, 0.45,  0.5, 0.5, 0.5, 1.0,
        0.35, 0.45,  0.5, 0.5, 0.5, 1.0,
        0.25, 0.75,  0.5, 0.5, 0.5, 1.0,
        0.35, 0.75,  0.5, 0.5, 0.5, 1.0,

        // Krov
        -0.5, 0.4,  1.0, 0.0, 0.0, 1.0,
        0.5, 0.4,   1.0, 0.0, 0.0, 1.0,
        0.0, 0.8,   1.0, 0.0, 0.0, 1.0,
    };
    unsigned int stride = 6 * sizeof(float);
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float circle[CRES * 2];
    float centerX = 0.3;
    float centerY = 0.2;
    float r = 0.05;
    for (int i = 0; i < CRES; i++)
    {
        float theta = (2.0 * 3.1415926f * i) / CRES;
        circle[2 * i] = centerX + r * cos(theta);
        circle[2 * i + 1] = centerY + r * sin(theta)* RATIO;
    }
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float windows_vertices[] =
    {
        //prozor
        0.25, 0.1, 0, 0, 1, 0.4,
        0.35, 0.1,  0, 0, 1, 0.4,
        0.25, 0.3,  0, 0, 1, 0.4,
        0.35, 0.3,   0, 0, 1, 0.4
    };
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(windows_vertices), windows_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float sun[CRES * 2];
    float sunX = -0.8;
    float sunY = 0.6;
    float sunR = 0.15;
    for (int i = 0; i < CRES; i++)
    {
        float theta = (2.0 * 3.1415926f * i) / CRES;
        sun[2 * i] = sunX + sunR * cos(theta);
        sun[2 * i + 1] = sunY + sunR * sin(theta) * RATIO;
    }
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sun), sun, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float tree_vertices[] =
    {
        //stablo
        0.76, -0.3, 0.36, 0.25, 0.20, 1.0,
        0.84, -0.3, 0.36, 0.25, 0.20, 1.0,
        0.76,  0.3, 0.36, 0.25, 0.20, 1.0,
        0.84,  0.3, 0.36, 0.25, 0.20, 1.0,

        0.65,  0.3, 0.36, 0.25, 0.20, 1.0,
        0.95,  0.3, 0.36, 0.25, 0.20, 1.0,
        0.65,  0.6, 0.36, 0.25, 0.20, 1.0,
        0.95,  0.6, 0.36, 0.25, 0.20, 1.0,
    };

    float colorProgress = 0.0f;

    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tree_vertices), tree_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int stride_image = 4 * sizeof(float);

    float image_vertices[] = {
        //slika imena
        -1.0, -0.65,   0.0, 1.0,
        -0.4, -0.65,   1.0, 1.0,
        -1.0, -1,   0.0, 0.0,  
        -0.4, -1,   1.0, 0.0   
    };


    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(image_vertices), image_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride_image, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride_image, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned checkerTexture = loadImageToTexture("ime.png");
    glBindTexture(GL_TEXTURE_2D, checkerTexture); 
    glGenerateMipmap(GL_TEXTURE_2D); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    unsigned uTexLoc = glGetUniformLocation(shaderImage, "uTex");
    glUniform1i(uTexLoc, 0); 

    float dog_vertices[] =
    {
        //pas
        -0.075,  -0.35, 0.0, 1.0, 0.0, 1.0,
         0.075,  -0.35, 0.0, 1.0, 0.0, 1.0,
        -0.075,  -0.45, 0.0, 1.0, 0.0, 1.0,
         0.075,  -0.45, 0.0, 1.0, 0.0, 1.0,

        -0.025,  -0.3, 0.0, 1.0, 0.0, 1.0,
         0.075,  -0.3, 0.0, 1.0, 0.0, 1.0,
        -0.025,  -0.45, 0.0, 1.0, 0.0, 1.0,
         0.075,  -0.45, 0.0, 1.0, 0.0, 1.0
    };

    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dog_vertices), dog_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float x = 0;
    float y = 0;
    float rotationAngle = 0;

    float fence_vertices[] = {
        //ograda
        -0.45, -0.4, 0.5, 0.2, 0.0, 1.0,
        -0.35, -0.4, 0.5, 0.2, 0.0, 1.0,
        -0.45, -0.6, 0.5, 0.2, 0.0, 1.0,
        -0.35, -0.6, 0.5, 0.2, 0.0, 1.0,

        -0.25, -0.4, 0.5, 0.2, 0.0, 1.0,
        -0.15, -0.4, 0.5, 0.2, 0.0, 1.0,
        -0.25, -0.6, 0.5, 0.2, 0.0, 1.0,
        -0.15, -0.6, 0.5, 0.2, 0.0, 1.0,

        -0.05, -0.4, 0.5, 0.2, 0.0, 1.0,
        0.05, -0.4, 0.5, 0.2, 0.0, 1.0,
        -0.05, -0.6, 0.5, 0.2, 0.0, 1.0,
        0.05, -0.6, 0.5, 0.2, 0.0, 1.0,

        0.15, -0.4, 0.5, 0.2, 0.0, 1.0,
        0.25, -0.4, 0.5, 0.2, 0.0, 1.0,
        0.15, -0.6, 0.5, 0.2, 0.0, 1.0,
        0.25, -0.6, 0.5, 0.2, 0.0, 1.0,

        0.35, -0.4, 0.5, 0.2, 0.0, 1.0,
        0.45, -0.4, 0.5, 0.2, 0.0, 1.0,
        0.35, -0.6, 0.5, 0.2, 0.0, 1.0,
        0.45, -0.6, 0.5, 0.2, 0.0, 1.0,
    };

    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fence_vertices), fence_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float smoke_vertices[] = {
        //dim
        0.28, 0.8,
        0.32, 0.85
    };

    float yPos = 0.0f;
    float speed = 0.00005f;
    unsigned int smoke_stride = 2 * sizeof(float);
    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(smoke_vertices), smoke_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, smoke_stride, (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float leaves_vertices[] = {
        0.65,  0.3, 0.0, 1.0, 0.0, 1.0,
        0.95,  0.3, 0.0, 1.0, 0.0, 1.0,
        0.65,  0.6, 0.0, 1.0, 0.0, 1.0,
        0.95,  0.6, 0.0, 1.0, 0.0, 1.0
    };

    glBindVertexArray(VAO[9]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(leaves_vertices), leaves_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    float background_vertices[] =
    {
        //trava
        -1, 0.0, 0.0, 0.8, 0.3, 1.0,
        1, 0.0,  0.0, 0.8, 0.3, 1.0,
        -1, -1, 0.0, 0.8, 0.3, 1.0,
        1, -1, 0.0, 0.8, 0.3, 1.0,

        //nebo
        -1, 0.0, 0.53, 0.81, 0.98, 1.0,
        1, 0.0,  0.53, 0.81, 0.98, 1.0,
        -1, 1,  0.53, 0.81, 0.98, 1.0,
        1, 1,   0.53, 0.81, 0.98, 1.0,
    };
    glBindVertexArray(VAO[10]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[10]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(background_vertices), background_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(5);
    glClearColor(1.0, 1.0, 1.0, 1.0);

    while (!glfwWindowShouldClose(window))
    {
        yPos += speed;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
        {
            glDisable(GL_BLEND);
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            x += 0.0001;
            if (x > 1.0)
                x = -1;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            x -= 0.0001;
            if (x < -1.0)
                x = 1.0;
        }

        if (yPos >= 0.3f) {
            yPos = 0.0f;
        }

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            colorProgress += 0.001f;
            if (colorProgress > 1.0f) {
                colorProgress = 1.0f;
            }
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            colorProgress -= 0.001f;
            if (colorProgress < 0.0f) {
                colorProgress = 0.0f;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        //pozadina
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(shader);
        glBindVertexArray(VAO[10]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

        //kuca
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(shader);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 24, 3);

        //cicaglisa
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(shaderGlisa);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(circle) / (2 * sizeof(float)));

        //prozor iza kog je cicaglisa
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(shader);
        glBindVertexArray(VAO[2]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        //sunce
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(shaderSun);
        glUniform1f(uA,glfwGetTime());
        glBindVertexArray(VAO[3]);
        glDrawArrays(GL_TRIANGLE_FAN, 0, sizeof(sun) / (2 * sizeof(float)));

        //drvo
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(shaderTrunk);
        glUniform1f(uColorProgressLoc, colorProgress);
        glBindVertexArray(VAO[4]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

        //krosnja
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(shaderLeaves);
        glBindVertexArray(VAO[9]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        //slika
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(shaderImage);
        glBindVertexArray(VAO[5]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, checkerTexture);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindTexture(GL_TEXTURE_2D, 0);

        //pas
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(shaderDog);
        glBindVertexArray(VAO[6]);
        glUniform2f(uPosLoc, x, y);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            glUniform1f(uRotationAngleLoc, 3.1415926f);
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            glUniform1f(uRotationAngleLoc, 0.0f);
        }
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

        //ograda
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(shader);
        glBindVertexArray(VAO[7]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);

        //dim
        glViewport(0, 0, wWidth, wHeight);
        glUseProgram(shaderSmoke);
        glBindVertexArray(VAO[8]);
        glUniform2f(uPosLoc, 0.0f, yPos);
        glDrawArrays(GL_POINTS, 0, sizeof(smoke_vertices) / smoke_stride);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(11, VBO);
    glDeleteVertexArrays(11, VAO);
    glDeleteProgram(shader);
    glDeleteProgram(shaderGlisa);
    glDeleteProgram(shaderSun);
    glDeleteProgram(shaderTrunk);
    glDeleteProgram(shaderImage);
    glDeleteProgram(shaderLeaves);
    glDeleteProgram(shaderSmoke);
    glDeleteProgram(shaderDog);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
    std::string temp = ss.str();
    const char* sourceCode = temp.c_str();
    int shader = glCreateShader(type);
    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;
    program = glCreateProgram();
    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}

static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}