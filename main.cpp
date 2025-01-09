#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <irrklang/irrKlang.h>

#include "stb/stb_image.h"

#include "shader.h"
#include "camera.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "sphere.h"

#include <iostream>

using namespace std;
using namespace irrklang;

ISoundEngine* SoundEngine = createIrrKlangDevice();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void drawCubeTexture(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, GLuint texture, float r, float g, float b);
void drawTriangle(unsigned int& triangleVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);


// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

bool shoot = false;
float bz = 0.01f;

float blt_z = 0.0f;
float head_z = 0.0f;
bool draw = true;

// camera
Camera camera(glm::vec3(1.0f, 1.5f, 15.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// direction of directional light
glm::vec3 directionalLight_direction = glm::vec3(-1.0f, 1.0f, -1.0f);

DirectionalLight directionalLight(-directionalLight_direction, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(-1.0f,  2.0f,  12.0f),  // Left Front light
    glm::vec3(2.5f,  2.0f,  12.0f),   // Right Front Light   
    glm::vec3(-1.5f,  3.0f,  0.0f),   // Left Back Light
    glm::vec3(2.5f,  3.0f,  0.0f),    // Right Back Light
    glm::vec3(-0.5f,  3.0f,  6.0f),   // Left Middle Light
    glm::vec3(1.5f,  3.0f,  6.0f)     // Right Middle Light
};

PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);

PointLight pointlight5(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight6(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool directionalLightOn = false;
bool pointLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 426: Computer Graphics Lab Final", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // --------------------------------------------------------------------- Cube

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, //0
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, //1
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, //2 
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //3 

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //4
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //5
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //6
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //7

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, //8
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //9
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //10
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 11

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //12
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //13
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //14
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //15

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //16
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //17
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //18
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //19

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //20
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //21
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //22
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //23
    };

    float road_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, //0
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, //1
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, //2 
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //3 

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //4
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //5
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //6
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //7

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, //8
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, //9
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, //10
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 11

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //12
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //13
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, //14
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, //15

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //16
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //17
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, //18
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, //19

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //20
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //21
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //22
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //23
    };

    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
    glEnableVertexAttribArray(2);

    // Load and create a texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture, and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load("res_wall_01_color.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // Road Buffer Arrays
    unsigned int roadVAO, roadVBO, roadEBO;
    glGenVertexArrays(1, &roadVAO);
    glGenBuffers(1, &roadVBO);
    glGenBuffers(1, &roadEBO);

    glBindVertexArray(roadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, roadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(road_vertices), road_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, roadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
    glEnableVertexAttribArray(2);

    GLuint road_texture;
    glGenTextures(1, &road_texture);
    glBindTexture(GL_TEXTURE_2D, road_texture);
    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture, and generate mipmaps
    //int width, height, nrChannels;
    data = stbi_load("road.jpeg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    GLuint hasina_texture;
    glGenTextures(1, &hasina_texture);
    glBindTexture(GL_TEXTURE_2D, hasina_texture);
    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture, and generate mipmaps
    //int width, height, nrChannels;
    data = stbi_load("hasina.jpeg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    GLuint sky_texture;
    glGenTextures(1, &sky_texture);
    glBindTexture(GL_TEXTURE_2D, sky_texture);
    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture, and generate mipmaps
    //int width, height, nrChannels;
    data = stbi_load("sky.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    GLuint screen_texture;
    glGenTextures(1, &screen_texture);
    glBindTexture(GL_TEXTURE_2D, screen_texture);
    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture, and generate mipmaps
    //int width, height, nrChannels;
    data = stbi_load("GAME.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // second, configure the light's VAO ------------------------------------ Light Cube
    unsigned int lightCubeVAO, lightVBO, lightEBO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &lightVBO);
    glGenBuffers(1, &lightEBO);

    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    //------------------------------------------------- 3D Triangle
    float triangle_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
        0.5f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, -1.0f, -1.0f, 1.0f, 0.0f,

        0.5f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
        0.0f, 0.0f, -1.0f, -1.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,

        1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
    };

    unsigned int triangleVAO, triangleVBO;
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &triangleVBO);

    glBindVertexArray(triangleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();
    //lightingShader.use();

    // Killer Hasina Song!
    ISound* killerSong = SoundEngine->play2D("killer_hasina.mp3", true);
    //killerSong->setIsPaused(true);

    float xTranslation = 0.0f;
    float yTranslation = 0.0f;
    float zTranslation = 0.0f;

    bool moveRight = true; // Direction for X-axis
    bool moveUp = true;    // Direction for Y-axis
    float movementSpeed = 0.002f; // Adjust as needed

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        pointlight1.setUpPointLight(lightingShader);
        pointlight2.setUpPointLight(lightingShader);
        pointlight3.setUpPointLight(lightingShader);
        pointlight4.setUpPointLight(lightingShader);
        pointlight5.setUpPointLight(lightingShader);
        pointlight6.setUpPointLight(lightingShader);

        directionalLight.setUpLight(lightingShader);

        // activate shader
        lightingShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);


        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));


        /*//Drawing a sphere
        Sphere sphere1 = Sphere();
                         //r    g     b      values
        sphere1.setColor(0.2f, 1.0f, 0.3f);
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.8f, 0.0f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.7f, 0.4f, 0.7f));
        model = translateMatrix * scaleMatrix;
        sphere1.drawSphere(lightingShader, model);


        //Drawing a triangle
        translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, -1.0f, -0.2f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5f, 0.7f, 1.0f));
        model = translateMatrix * scaleMatrix;
                                                         //r    g     b      values
        drawTriangle(triangleVAO, lightingShader, model, 0.8f, 0.3f, 1.0f);


        //Drawing a cube
        translateMatrix = glm::translate(identityMatrix, glm::vec3(1.8f, -0.4f, -0.6f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
        model = translateMatrix * scaleMatrix;
                                                 //r    g     b      values
        drawCube(cubeVAO, lightingShader, model, 0.1f, 0.6f, 1.0f);*/

        if (draw) {
            // --------------------------------------- Sky -----------------
            // Blue Cube
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-17.0f, -10.0f, -15.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(35.0f, 25.0f, 1.0f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, sky_texture, 1.0f, 1.0f, 1.0f);


            // --------------------------------------- Flag -----------------
            // Green Cube
            //translateMatrix = glm::translate(identityMatrix, glm::vec3(0.2f, 4.0f, -5.6f));
            //scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0f, 1.5f, 1.0f));
            //model = translateMatrix * scaleMatrix;
            ////r    g     b      values
            //drawCube(cubeVAO, lightingShader, model, 0.0f, 1.0f, 0.0f);

            //// Red Circle
            //Sphere sphere1 = Sphere();
            //                 //r    g     b      values
            //sphere1.setColor(1.0f, 0.0f, 0.0f);
            //translateMatrix = glm::translate(identityMatrix, glm::vec3(1.2f, 4.8f, -4.6f));
            //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.6f, 0.4f, 0.6f));
            //model = translateMatrix * scaleMatrix;
            //sphere1.drawSphere(lightingShader, model);


            // --------------------------------------- Road ---------------------------------------------------------------------
            // Road
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.5f, 0.0f, 0.3f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0f, 0.2f, 12.0f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(roadVAO, lightingShader, model, road_texture, 0.5f, 0.5f, 0.5f);


            // Obstacles triangle
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.2f, 5.2f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.1f, 0.2f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawTriangle(triangleVAO, lightingShader, model, 1.0f, 0.0f, 0.0f);

            // Obstacles triangle
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 0.2f, 4.2f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.1f, 0.2f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawTriangle(triangleVAO, lightingShader, model, 1.0f, 1.0f, 1.0f);


            // -------------------------------------- Buildings in Right side of road   ---------------------------------------------
            // Building 1
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, -0.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.5f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.7f, 0.0f, 1.0f);

            // Building 2
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, -0.0f, 0.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.2f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.0f, 0.0f, 1.0f);

            // Building 3
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, 1.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.0f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.0f, 1.0f, 1.0f);

            // Building 4
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, 2.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.3f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 1.0f, 0.0f, 1.0f);

            // Building 5
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, 3.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.0f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.0f, 1.0f, 0.0f);

            // Building 6
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, 4.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.5f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.0f, 0.0f, 1.0f);

            // Building 7
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, 5.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.2f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.0f, 1.7f, 0.7f);

            // Building 8
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, 6.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.4f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 1.0f, 1.0f, 1.0f);

            // Building 9
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, 7.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.6f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.0f, 1.2f, 0.7f);

            // Building 10
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, 8.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.1f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.4f, 1.3f, 1.8f);


            // Building 11
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, 9.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.5f, 0.8f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.6f, 1.9f, 0.5f);

            // Building 12
            translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.0f, 10.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.6f, 0.8f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 1.0f, 1.0f, 1.0f);

            //   ------------------------------------- Buildings in Left side of Road -----------------------------------------------
            // Building 1
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, -0.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.5f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.6f, 1.9f, 0.5f);

            // Building 2
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 0.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.7f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.0f, 1.2f, 0.7f);

            // Building 3
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 1.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.5f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.0f, 1.7f, 0.7f);

            // Building 4
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 2.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 1.9f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.4f, 1.3f, 1.8f);

            // Building 5
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 3.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.3f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 1.0f, 1.0f, 1.0f);

            // Building 6
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 4.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.4f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 1.0f, 0.0f, 1.0f);

            // Building 7
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 5.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.3f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.0f, 1.0f, 1.0f);

            // Building 8
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 6.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.4f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 1.0f, 1.0f, 1.0f);

            // Building 9
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 7.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.3f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 0.0f, 1.0f, 1.0f);

            // Building 10
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 8.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.5f, 0.6f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 1.5f, 0.8f, 1.9f);

            // Building 11
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 9.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.4f, 0.8f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 1.0f, 1.0f, 1.0f);

            // Building 12
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.3f, 0.0f, 10.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 2.6f, 0.8f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, texture, 1.0f, 1.0f, 1.0f);

            // ---------------------------------------- KIller Hasina -----------------------
            /*if (zTranslation < 8) {
                zTranslation += 0.003f;
            }*/
            if (zTranslation < 8) {
                zTranslation += movementSpeed; // Z moves forward constantly
            }


            // Update X-axis translation
            if (moveRight) {
                xTranslation += movementSpeed;
                if (xTranslation > 0.5f) { // Adjust the threshold as needed
                    moveRight = false;
                }
            }
            else {
                xTranslation -= movementSpeed;
                if (xTranslation < -0.5f) { // Adjust the threshold as needed
                    moveRight = true;
                }
            }

            // Update Y-axis translation
            if (moveUp) {
                yTranslation += movementSpeed;
                if (yTranslation > 0.5f) { // Adjust the threshold as needed
                    moveUp = false;
                }
            }
            else {
                yTranslation -= movementSpeed;
                if (yTranslation < 0.2f) { // Adjust the threshold as needed
                    moveUp = true;
                }
            }

            // 1. Head
            /*translateMatrix = glm::translate(identityMatrix, glm::vec3(0.7f, 1.0f, 0.5f+zTranslation));*/
            head_z = 0.5f + zTranslation;
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.7f + xTranslation, 1.0f + yTranslation, head_z));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCubeTexture(cubeVAO, lightingShader, model, hasina_texture, 1.0f, 1.0f, 1.0f);

            // 2. Neck
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.875f + xTranslation, 0.8f + yTranslation, 0.5f + zTranslation));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 0.2f, 0.5f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCube(cubeVAO, lightingShader, model, 1.0f, 0.0f, 0.0f);

            // 3. Body
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.55f + xTranslation, 0.4f + yTranslation, 0.5f + zTranslation));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 0.5f, 0.51f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCube(cubeVAO, lightingShader, model, 1.0f, 0.0f, 0.0f);

            // 4. Left Hand
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.1f + xTranslation, 0.7f + yTranslation, 0.5f + zTranslation));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 0.05f, 0.5f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCube(cubeVAO, lightingShader, model, 1.0f, 01.0f, 01.0f);

            // 5. Right Hand
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f + xTranslation, 0.7f + yTranslation, 0.5f + zTranslation));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.8f, 0.05f, 0.5f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCube(cubeVAO, lightingShader, model, 1.0f, 01.0f, 01.0f);

            // 6. left Leg
            translateMatrix = glm::translate(identityMatrix, glm::vec3(0.7f + xTranslation, 0.0f + yTranslation, 0.5f + zTranslation));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 0.5f, 0.5f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCube(cubeVAO, lightingShader, model, 1.0f, 1.0f, 1.0f);

            // 7. Right Leg
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f + xTranslation, 0.0f + yTranslation, 0.5f + zTranslation));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.15f, 0.5f, 0.5f));
            model = translateMatrix * scaleMatrix;
            //r    g     b      values
            drawCube(cubeVAO, lightingShader, model, 1.0f, 1.0f, 1.0f);


            // ----------------------------------------- Gun ---------------------------------------------------------------
            // Body (Pipe)
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 1.5f, 10.6f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.05f, 1.5f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 0.1f, 0.6f, 1.0f);

            // Handle
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 1.35f, 12.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.21f, 0.10f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 1.0f, 0.0f, 0.0f);

            // Switch
            translateMatrix = glm::translate(identityMatrix, glm::vec3(1.0f, 1.45f, 11.8f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.08f, 0.05f, 0.3f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 1.0f, 1.0f, 1.0f);

            if (bz < 8) {
                bz += 0.03f;
            }
            else {
                shoot = false;
                bz = 0;
            }


            // Bullet
            if (!shoot) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.01f, 1.51f, 10.5f));
            }
            else {
                blt_z = 10.5f - bz;
                translateMatrix = glm::translate(identityMatrix, glm::vec3(1.01f, 1.51f, blt_z));
            }
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.035f, 0.02f, 0.15f));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 1.0f, 1.0f, 1.0f);

        }

        if (shoot) {
            if (head_z <= blt_z && blt_z <= (head_z + 0.13)) {
                translateMatrix = glm::translate(identityMatrix, glm::vec3(-16.875f, -10.8f, 5.0f));
                scaleMatrix = glm::scale(identityMatrix, glm::vec3(30.15f, 30.2f, 1.5f));
                model = translateMatrix * scaleMatrix;
                //r    g     b      values
                drawCubeTexture(cubeVAO, lightingShader, model, screen_texture, 1.0f, 1.0f, 1.0f);
                draw = false;
                //killerSong->setIsPaused(true);
                std::printf("Stop\n");
            }
        }





        // Also draw the lamp object(s)
        // The white cubes are lamp objects
        // As we can't see the point where the point lights are, we are representing the points with the white cubes
        // We don't need the lighting effects on the cubes, that's why we are using simple shader, with only one color element
        // fragmentShader.fs and vertexShader.fs are the simple shaders codes. 
        // SEE THESE TWO FILES!
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 6; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            if (pointLightOn)
                ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            else
                ourShader.setVec3("color", glm::vec3(0.25f, 0.25f, 0.25f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setBool("useTexture", false);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawCubeTexture(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), GLuint texture = (GLuint)0, float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setBool("useTexture", true);

    lightingShader.setMat4("model", model);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void drawTriangle(unsigned int& triangleVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(triangleVAO);
    glDrawArrays(GL_TRIANGLES, 0, 24);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if (!shoot) {
            shoot = true;
        }
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        if (directionalLightOn)
        {
            directionalLight.turnOff();
            directionalLightOn = !directionalLightOn;
        }
        else
        {
            directionalLight.turnOn();
            directionalLightOn = !directionalLightOn;
        }
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointlight5.turnOff();
            pointlight6.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointlight5.turnOn();
            pointlight6.turnOn();
            pointLightOn = !pointLightOn;
        }
    }


    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        if (specularToggle)
        {

            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            pointlight5.turnSpecularOff();
            pointlight6.turnSpecularOff();

            specularToggle = !specularToggle;
        }
        else
        {

            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            pointlight5.turnSpecularOn();
            pointlight6.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }

    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        if (diffuseToggle)
        {

            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            pointlight5.turnDiffuseOff();
            pointlight6.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {

            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            pointlight5.turnDiffuseOn();
            pointlight6.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }

    else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        if (ambientToggle)
        {

            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            pointlight5.turnAmbientOff();
            pointlight6.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {

            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            pointlight5.turnAmbientOn();
            pointlight6.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
