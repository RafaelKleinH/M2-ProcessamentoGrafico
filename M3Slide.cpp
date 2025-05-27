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

struct Box {
    GLuint VAO;
    float r, g, b;
    bool deleted;
};

vector<Box> model;
const int HEIGHT = 800;
const int WIDTH = 1080;
const float GRID_COLUMNS = 12;
const float GRID_LINES = 8;
const float GRID_COLUMNS_SUM = 2 / GRID_COLUMNS;
const float GRID_LINES_SUM = 2 / GRID_LINES;

void macOSInit() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

float generateRandomNumber() {
    float randomNum = rand() % 101;
    return randomNum / 100;
}

float cursorX = 0;
float cursorY = 0;

Box createBox(float x0, float y0, float x1, float y1) {

    float r = generateRandomNumber();
    float g = generateRandomNumber();
    float b = generateRandomNumber();
    GLfloat vertices[] = {
        x0, y0, 0.0,
        x1, y1, 0.0,
        x0, y1, 0.0,

        x0, y0, 0.0,
        x1, y1, 0.0,
        x1, y0, 0.0};

    GLfloat colors[] = {
        r,
        g,
        b,
        r,
        g,
        b,
        r,
        g,
        b,
        r,
        g,
        b,
        r,
        g,
        b,
        r,
        g,
        b,
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

    Box box;
    box.VAO = VAO;
    box.r = r;
    box.g = g;
    box.b = b;
    box.deleted = false;
    return box;
}
bool pressed = false;
bool needToDelete = false;

void createModel() {
    float y0 = -1;
    float y1 = -1 + GRID_LINES_SUM;
    for (int y = 0; y < GRID_LINES; y++)
    {
        float x0 = -1;
        float x1 = -1 + GRID_COLUMNS_SUM;
        for (int x = 0; x < GRID_COLUMNS; x++)
        {
            Box box = createBox(x0, y0, x1, y1);
            model.push_back(box);
            x0 = x0 + GRID_COLUMNS_SUM;
            x1 = x1 + GRID_COLUMNS_SUM;
        }
        y0 = y0 + GRID_LINES_SUM;
        y1 = y1 + GRID_LINES_SUM;
    }
}

int getModelIndex() {
    float newValueX = cursorX + 1;
    float newValueY = cursorY + 1;
    float a = newValueX / GRID_COLUMNS_SUM;
    float b = newValueY / GRID_LINES_SUM;
    float rows = int(b) * GRID_COLUMNS;
    float item = int(a);
    int index = rows + item;

    cout << "X: " << cursorX << endl;
    cout << "Y: " << cursorY << endl;
    cout << "a: " << a << endl;
    cout << "b: " << b << endl;
    cout << "row: " << rows << endl;
    cout << "item: " << item << endl;

    return index;
}

void deleteContent() {

    int index = getModelIndex();
    model[index].deleted = needToDelete;
 
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        pressed = true;
        needToDelete = true;
        //    deleteContent();
    }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        pressed = true;
        needToDelete = false;
        //    deleteContent();
    }
    
     if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        pressed = false;
     }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        pressed = false;
     }
}

void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos) {
    float oldRangeX = (WIDTH - 0);
    float newRangeX = (1 - (-1));
    float newValueX = (((xPos - 0) * newRangeX) / oldRangeX) + (-1);

    float oldRangeY = (HEIGHT - 0);
    float newRangeY = (1 - (-1));
    float newValueY = ((((HEIGHT - yPos) - 0) * newRangeY) / oldRangeY) + (-1);

    if (newValueX < -1) {
        cursorX = -1; 
    } else if (newValueX > 1) {
        cursorX = 1;
    } else {
        cursorX = newValueX;
    }

        if (newValueY < -1) {
        cursorY = -1; 
    } else if (newValueY > 1) {
        cursorY = 1;
    } else {
        cursorY = newValueY;
    }

    if (pressed) {
        deleteContent();
    }
}

int main() {

    glfwInit();
    macOSInit();

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Triangulo 3 o inimigo agora são as cores", NULL, NULL);
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

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    createModel();

    while (!glfwWindowShouldClose(window)) { // Enquanto nao fechar ele fica em um looping

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        for (int i = 0; i < model.size(); i++) {

            if (!model[i].deleted) {
                glBindVertexArray(model[i].VAO);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }
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
