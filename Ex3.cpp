// GLAD
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

const char *vertexShaderSource = "#version 410\n"
                                 "layout(location=0) in vec3 vp;" // layout 0 foi utilizado para receber vec3 do vértice
                                 "layout(location=1) in vec3 vc;" // layout 1 foi utilizado para receber vec3 da cor
                                 "out vec3 color;"                // todos os estágios tem: entra -> proc -> saída, color é a saída para ao FS
                                 "void main () {"
                                 "   color = vc;"                    // a cor vc associada ao respectivo vértice vp é propagada para o FS
                                 "	  gl_Position = vec4 (vp, 1.0);" // objetivo do VS, posicionar o vértice vp no sistema de referência do universo
                                 "}";

const char *fragmentShaderSource = "#version 410\n"
                                   "in vec3 color;" // recebe a cor saída do estágio anterior VS
                                   "out vec4 frag_color;"
                                   "void main () {"
                                   "	 frag_color = vec4 (color, 1.0);" // objetivo do FS, determinar a cor do fragmento
                                   "}";

const int size = 1080;

void macOSInit()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

float generateRandomNumber()
{
    float randomNum = rand() % 101;
    return randomNum / 100;
}

struct Triangle
{
    float x, y, r, g, b;
};

struct CreateTriangleModel
{
    Triangle triangle;
    GLuint VAO;
};

vector<CreateTriangleModel> model;
float cursorX = 0;
float cursorY = 0;

Triangle generateTriangle(float cursorX, float cursorY)
{
    Triangle triangle;
    triangle.x = cursorX;
    triangle.y = cursorY;
    triangle.r = generateRandomNumber();
    triangle.g = generateRandomNumber();
    triangle.b = generateRandomNumber();
    return triangle;
}

GLuint createTriangle(float x0, float y0, float x1, float y1, float x2, float y2, Triangle triangle)
{ //-0.1, -0.1, 0.1, -0.1, 0.0, 0.1

    float oldRange = (1 - 0);
    float newRange = (1 - (-1));
    float newValueX = (((triangle.x - 0) * newRange) / oldRange) + (-1);
    float newValueY = (((triangle.y - 0) * newRange) / oldRange) + (-1);

    GLfloat vertices[] = {
        newValueX + x0, newValueY + y0, 0.0f,
        newValueX + x1, newValueY + y1, 0.0f,
        newValueX, newValueY + y2, 0.0f};

    GLfloat colors[] = {
        triangle.r,
 triangle.g,
 triangle.b,
        triangle.r,
 triangle.g,
 triangle.b,
          triangle.r,
 triangle.g,
 triangle.b,
    };

    GLuint VAO, VBO, VBOcolor;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBOcolor);
    glBindBuffer(GL_ARRAY_BUFFER, VBOcolor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, VBOcolor);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    return VAO;
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        CreateTriangleModel abc;
        abc.triangle = generateTriangle(cursorX, cursorY);
        abc.VAO = createTriangle(-0.1, -0.1, 0.1, -0.1, 0.0, 0.1, abc.triangle);
        model.push_back(abc);
    }
}

void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos)
{
    float oldRange = (size - 0);
    float newRange = (1 - 0);
    float newValueX = (((xPos - 0) * newRange) / oldRange) + 0;
    float newValueY = ((((size - yPos) - 0) * newRange) / oldRange) + 0;

    if (newValueX < 0)
    {
        cursorX = 0;
    }
    else if (newValueX > size)
    {
        cursorX = size;
    }
    else
    {
        cursorX = newValueX;
    }

    if (newValueY < 0)
    {
        cursorY = 0;
    }
    else if (newValueY > size)
    {
        cursorY = size;
    }
    else
    {
        cursorY = newValueY;
    }
}

int main()
{

    glfwInit();
    macOSInit();

    GLFWwindow *window = glfwCreateWindow(size, size, "Triangulo 2 o inimigo agora é o mesmo * 5", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "*** ERRO: não foi possível abrir janela com a GLFW\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    // gladLoadGL();
    glewExperimental = GL_TRUE;
    glewInit();

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);     // cria shader falando que é de vertice
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // passa qual o shader source pro shader que criamos
    glCompileShader(vertexShader);                              // compila o shader

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // msm coisa de cima mas é fragment dessa vez
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram(); // cria um shader program e passa os shader pra ele
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram); // linka o programa pra ele

    // glDeleteShader(vertexShader); // pode deletar pq foi linkado
    // glDeleteShader(fragmentShader); // pode deletar pq foi linkado

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window))
    { // Enquanto nao fechar ele fica em um looping

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        for (int i = 0; i < model.size(); i++)
        {

            glBindVertexArray(model[i].VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        // Fala pro glfw processar todos eventos na tela dele (window que criamos)
    }
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
