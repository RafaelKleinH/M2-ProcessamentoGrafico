// GLAD
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <stdio.h> 

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos; \n"
"void main()\n"
"{\n"
" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor; \n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

void macOSInit()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2) {
    GLfloat vertices[] = {
        x0, y0, 0.0f,
        x1, y1, 0.0f,
        x2, y2, 0.0f
    };

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

int main()
{

    glfwInit();
    macOSInit();

    GLFWwindow *window = glfwCreateWindow(800, 800, "Triangulo 2 o inimigo agora é o mesmo * 5", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "*** ERRO: não foi possível abrir janela com a GLFW\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL(); 

    glViewport(0,0, 800, 800);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); // cria shader falando que é de vertice
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // passa qual o shader source pro shader que criamos
    glCompileShader(vertexShader); // compila o shader

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // msm coisa de cima mas é fragment dessa vez
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram(); // cria um shader program e passa os shader pra ele
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram); // linka o programa pra ele

    glDeleteShader(vertexShader); // pode deletar pq foi linkado
    glDeleteShader(fragmentShader); // pode deletar pq foi linkado

    GLuint VAO[] = { createTriangle(0, 0.5, -0.5, 1, 0.5, 1), createTriangle(0, 0.5, -0.5, 0, 0.5, 0), createTriangle(-0.5, 0, -1, 0.5, -1, -0.5), createTriangle(0.5, 0, 1, 0.5, 1, -0.5), createTriangle(0, 0, -1, -1, 1, -1) };

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window)) { // Enquanto nao fechar ele fica em um looping

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        for (int i = 0; i < sizeof(VAO); i++) {
        glBindVertexArray(VAO[i]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
         glfwSwapBuffers(window);
        glfwPollEvents(); // Fala pro glfw processar todos eventos na tela dele (window que criamos)
    }
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
