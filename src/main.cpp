#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#include <headers/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <headers/Shader.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "ShapesDrawing.h"
#include "BuffersInit.h"
#include "Particle.h"
#include "Defines.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadCubemap(std::vector<std::string> faces);
void generateTex(unsigned int texture, const char* path);
// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 1000;
// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
float fov = 45.0f;
float currentX = 0.0f, currentZ = 0.0f;
float shipMovement = 0.0f, smokeSpeed = 0.0f, acceleration = 0.005f, rotation = 0.0f;
bool stand = true, forward = false, backward = false, left = false, right = false;
// lighting
glm::vec3 lightPos(1.2f, 2.0f, 5.0f);
Particle particle;
glm::vec3 partBeg;


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Parowiec", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````shaders
	Shader ourShader("Shaders\\shader.vs", "Shaders\\shader.fs"),
		colorShader("Shaders\\shader_c.vs", "Shaders\\shader_c.fs"),
		particleShader("Shaders\\particle.vs", "Shaders\\particle.fs");
	
	unsigned VAO, VBO, VAODeck, VBODeck, VAOFloor, VBOFloor, VAOCrane, VBOCrane,
		EBOCrane, VAO_cc, VBO_cc, VAO_b, VBO_b, VAO_rr, VBO_rr, VAO_pp, VBO_pp,
		VAO_circle, VBO_circle, VAO_circle_c, VBO_circle_c, VAO_c1, VBO_c1, EBO_c1,
		VAOSea, VBOSea, VBOCrate, VAOCrate, VAOParticle, VBOParticle;

	glm::vec3 left_bottom = { LEFT_X, BOTTOM_LEVEL, MIDDLE_Z };
	glm::vec3 right_bottom = { LEFT_X + LENGTH, BOTTOM_LEVEL, MIDDLE_Z };
	glm::vec3 left_up = { LEFT_X, DECK_LEVEL - DECK_HEIGHT, MIDDLE_Z + 0.5*WIDTH };
	glm::vec3 bukszpryt = { LEFT_X + 1.5*LENGTH, DECK_LEVEL, MIDDLE_Z };

	std::vector<float> bottom = draw_bottom(left_bottom, right_bottom, left_up, bukszpryt, PRECISION);
	prepareBottom(bottom, VAO, VBO);

	std::vector<float> deck = draw_deck(bukszpryt);
	prepareDeck(deck, VAODeck, VBODeck);

	std::vector<float> floor = draw_floor(bukszpryt);
	prepareLightedShape(floor, VAOFloor, VBOFloor);

	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````crane
	std::vector<float> vertices_crane = {
		//positions              //normals            //texture
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f, 0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f, 0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f, 0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-0.5f, 0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, 0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, 0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, 0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, 0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f, 0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		0.5f, 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, 0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f
	};
	prepareLightedShape(vertices_crane, VAOCrane, VBOCrane);
	
	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````cylinder chimney
	std::vector<float> vertices_cc = draw_cylinder(0.0f, CHIMNEY_HEIGHT, 0.0f, CHIMNEY_RADIUS, 0.7*CHIMNEY_RADIUS, 0.6*CHIMNEY_RADIUS, PRECISION);
	prepareLightedShape(vertices_cc, VAO_cc, VBO_cc);

	//`````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````barriers
	std::vector<float> vertices_b = draw_cylinder(0.0f, BARRIER_HEIGHT, 0.0f, BARRIER_RADIUS, BARRIER_RADIUS, 0, PRECISION);
	prepareLightedShape(vertices_b, VAO_b, VBO_b);
	
	float RX = 0.0f;
	float RY = 0.0f;
	float RZ = 0.0f;
	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````rotator
	std::vector<float> vertices_rr = draw_rotator(RX, RY, RZ, ROTATOR_RADIUS, 1.1f*ROTATOR_RADIUS, ROTATOR_WIDTH, PRECISION);
	prepareLightedShape(vertices_rr, VAO_rr, VBO_rr);
	

	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````paddles
	float p_width = 0.02f;
	std::vector<float> vertices_pp = draw_paddles(RX, RY, RZ, 0.8f*ROTATOR_RADIUS, p_width, ROTATOR_WIDTH_2);
	prepareLightedShape(vertices_pp, VAO_pp, VBO_pp);

	
	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````circle rotators
	std::vector<float> vertices_circle = draw_circle(RX, RY, RZ, CIRCLE_RADIUS, PRECISION);
	prepareLightedShape(vertices_circle, VAO_circle, VBO_circle);


	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````circle chimney bottom
	std::vector<float> vertices_circle_c = draw_circle(RX, RY, RZ, CHIMNEY_RADIUS, PRECISION);
	prepareLightedShape(vertices_circle_c, VAO_circle_c, VBO_circle_c);

	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````cuboid
	std::vector<glm::vec3> vertices_c1 = draw_cuboid_v(0, 0, 0, 0.4f, 0.3f, 0.3f);
	std::vector<unsigned int> indices_c1 = draw_cuboid_i();
	prepareCuboid(vertices_c1, indices_c1, VAO_c1, VBO_c1, EBO_c1);
	

	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````sea
	Shader shaderSea("Shaders\\shaderTex.vs", "Shaders\\shaderTex.fs");
	std::vector<float> verticesSea = {
		-1000.0f, SEA_LEVEL,  1000.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,
		1000.0f, SEA_LEVEL,  1000.0f,	0.0f, 1.0f, 0.0f,	300.0f, 0.0f,
		-1000.0f, SEA_LEVEL, -1000.0f,	0.0f, 1.0f, 0.0f,	0.0f, 300.0f,
		1000.0f, SEA_LEVEL,  1000.0f,	0.0f, 1.0f, 0.0f,	300.0f, 0.0f,
		1000.0f, SEA_LEVEL, -1000.0f,	0.0f, 1.0f, 0.0f,	300.0f, 300.0f,
		-1000.0f, SEA_LEVEL, -1000.0f,	0.0f, 1.0f, 0.0f,	0.0f, 300.0f,
	};

	prepareLightedShape(verticesSea, VAOSea, VBOSea);


	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````crate
	std::vector<float> verticesCrate = {
		//positions								//normals            //texture
		-CRATE_SIZE, -CRATE_SIZE,  CRATE_SIZE,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,// przod
		CRATE_SIZE, -CRATE_SIZE,  CRATE_SIZE,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		CRATE_SIZE,  CRATE_SIZE,  CRATE_SIZE,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		CRATE_SIZE,  CRATE_SIZE,  CRATE_SIZE,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-CRATE_SIZE,  CRATE_SIZE,  CRATE_SIZE,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-CRATE_SIZE, -CRATE_SIZE,  CRATE_SIZE,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-CRATE_SIZE,  CRATE_SIZE,  CRATE_SIZE, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,// lewo
		-CRATE_SIZE,  CRATE_SIZE, -CRATE_SIZE, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-CRATE_SIZE, -CRATE_SIZE, -CRATE_SIZE, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-CRATE_SIZE, -CRATE_SIZE, -CRATE_SIZE, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-CRATE_SIZE, -CRATE_SIZE,  CRATE_SIZE, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-CRATE_SIZE,  CRATE_SIZE,  CRATE_SIZE, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		CRATE_SIZE,  CRATE_SIZE,  CRATE_SIZE,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,// prawo
		CRATE_SIZE,  CRATE_SIZE, -CRATE_SIZE,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		CRATE_SIZE, -CRATE_SIZE, -CRATE_SIZE,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		CRATE_SIZE, -CRATE_SIZE, -CRATE_SIZE,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		CRATE_SIZE, -CRATE_SIZE,  CRATE_SIZE,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		CRATE_SIZE,  CRATE_SIZE,  CRATE_SIZE,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-CRATE_SIZE,  CRATE_SIZE, -CRATE_SIZE,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,//gra
		CRATE_SIZE,  CRATE_SIZE, -CRATE_SIZE,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		CRATE_SIZE,  CRATE_SIZE,  CRATE_SIZE,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		CRATE_SIZE,  CRATE_SIZE,  CRATE_SIZE,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		-CRATE_SIZE,  CRATE_SIZE,  CRATE_SIZE,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		-CRATE_SIZE,  CRATE_SIZE, -CRATE_SIZE,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-CRATE_SIZE, -CRATE_SIZE, -CRATE_SIZE,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,// ty
		CRATE_SIZE, -CRATE_SIZE, -CRATE_SIZE,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		CRATE_SIZE,  CRATE_SIZE, -CRATE_SIZE,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		CRATE_SIZE,  CRATE_SIZE, -CRATE_SIZE,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-CRATE_SIZE,  CRATE_SIZE, -CRATE_SIZE,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-CRATE_SIZE, -CRATE_SIZE, -CRATE_SIZE,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		-CRATE_SIZE, -CRATE_SIZE, -CRATE_SIZE,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,// d
		CRATE_SIZE, -CRATE_SIZE, -CRATE_SIZE,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		CRATE_SIZE, -CRATE_SIZE,  CRATE_SIZE,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		CRATE_SIZE, -CRATE_SIZE,  CRATE_SIZE,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		-CRATE_SIZE, -CRATE_SIZE,  CRATE_SIZE,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		-CRATE_SIZE, -CRATE_SIZE, -CRATE_SIZE,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f
	};
	prepareLightedShape(verticesCrate, VAOCrate, VBOCrate);
	

	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````textures
	unsigned int textureSea, textureCrate, textureCrane, textureChimney,
		textureRotator, textureBottom, texturePaddles, textureCircle, textureDeck, textureFloor,
		textureWall1, textureWall2, textureWall3, textureWall4, textureWall5, textureIsland, textureTree, textureLeaves;
	glGenTextures(1, &textureSea);
	generateTex(textureSea, "Pictures\\sea.jpg");
	glGenTextures(1, &textureCrate);
	generateTex(textureCrate, "Pictures\\crate.jpg");
	glGenTextures(1, &textureCrane);
	generateTex(textureCrane, "Pictures\\crane3.jpg");
	glGenTextures(1, &textureChimney);
	generateTex(textureChimney, "Pictures\\chimney.jpg");
	glGenTextures(1, &textureRotator);
	generateTex(textureRotator, "Pictures\\rotator.jpg");
	glGenTextures(1, &texturePaddles);
	generateTex(texturePaddles, "Pictures\\paddles2.jpg");
	glGenTextures(1, &textureCircle);
	generateTex(textureCircle, "Pictures\\anchor.jpg");
	glGenTextures(1, &textureDeck);
	generateTex(textureDeck, "Pictures\\black_texture.jpg");
	glGenTextures(1, &textureBottom);
	generateTex(textureBottom, "Pictures\\red_texture.jpg");
	glGenTextures(1, &textureFloor);
	generateTex(textureFloor, "Pictures\\floor.jpg");
	glGenTextures(1, &textureWall1);
	generateTex(textureWall1, "Pictures\\wall1.jpg");
	glGenTextures(1, &textureWall2);
	generateTex(textureWall2, "Pictures\\wall2.jpg");
	glGenTextures(1, &textureWall3);
	generateTex(textureWall3, "Pictures\\wall3.jpg");
	glGenTextures(1, &textureWall4);
	generateTex(textureWall4, "Pictures\\wall4.jpg");
	glGenTextures(1, &textureWall5);
	generateTex(textureWall5, "Pictures\\wall5.jpg");
	glGenTextures(1, &textureIsland);
	generateTex(textureIsland, "Pictures\\island.jpg");
	glGenTextures(1, &textureTree);
	generateTex(textureTree, "Pictures\\tree.jpg");
	glGenTextures(1, &textureLeaves);
	generateTex(textureLeaves, "Pictures\\leaves.jpg");

	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````light
	Shader lightingShader("Shaders\\1.colors.vs", "Shaders\\1.colors.fs");
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOCrate);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````skybox
	Shader skyboxShader("Shaders\\skybox.vs", "Shaders\\skybox.fs");
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	float skyboxVertices[] = {
		// positions        
		1.0f, -0.1f, -1.0f,//right
		1.0f, -0.1f,  1.0f,
		1.0f,  1.5f,  1.0f,
		1.0f,  1.5f,  1.0f,
		1.0f,  1.5f, -1.0f,
		1.0f, -0.1f, -1.0f,
		-1.0f, -0.1f,  1.0f,//left
		-1.0f, -0.1f, -1.0f,
		-1.0f,  1.5f, -1.0f,
		-1.0f,  1.5f, -1.0f,
		-1.0f,  1.5f,  1.0f,
		-1.0f, -0.1f,  1.0f,
		-1.0f,  1.5f, -1.0f,//top
		1.0f,  1.5f, -1.0f,
		1.0f,  1.5f,  1.0f,
		1.0f,  1.5f,  1.0f,
		-1.0f,  1.5f,  1.0f,
		-1.0f,  1.5f, -1.0f,
		-1.0f, -0.1f, -1.0f,//bottom
		-1.0f, -0.1f,  1.0f,
		1.0f, -0.1f, -1.0f,
		1.0f, -0.1f, -1.0f,
		-1.0f, -0.1f,  1.0f,
		1.0f, -0.1f,  1.0f,
		-1.0f, -0.1f,  1.0f,//front
		-1.0f,  1.5f,  1.0f,
		1.0f,  1.5f,  1.0f,
		1.0f,  1.5f,  1.0f,
		1.0f, -0.1f,  1.0f,
		-1.0f, -0.1f,  1.0f,
		-1.0f,  1.5f, -1.0f,//back
		-1.0f, -0.1f, -1.0f,
		1.0f, -0.1f, -1.0f,
		1.0f, -0.1f, -1.0f,
		1.0f,  1.5f, -1.0f,
		-1.0f,  1.5f, -1.0f
	};

	//`````````````````````````````````````````sky
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	std::vector<std::string> faces
	{
		"Pictures\\sky\\right.jpg",
		"Pictures\\sky\\left.jpg",
		"Pictures\\sky\\top.jpg",
		"Pictures\\sky\\bottom.jpg",
		"Pictures\\sky\\front.jpg",
		"Pictures\\sky\\back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	float paddle_angle = 0.0f;

	//``````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````patricles
	
	prepareLightedShape(verticesCrate, VAOParticle, VBOParticle);
	
	particle.initParticles();

	//``````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````MAIN LOOP
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);
		if (!stand)
		{
			if (forward)
			{
				shipMovement += acceleration;
				paddle_angle -= 100 * acceleration + 1.0f;
			}
			else if (backward)
			{
				shipMovement -= acceleration;
				paddle_angle += 100 * acceleration + 1.0f;
			}
		}
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glm::mat4 m1, m2, m3;
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 model = glm::mat4(1.0f);

		lightingShader.use();
		lightingShader.setVec3("viewPos", cameraPos);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setVec3("material.specular", 0.4f, 0.4f, 0.4f);
		lightingShader.setVec3("dirLight.direction", -3.2f, -2.0f, -5.0f);
		lightingShader.setVec3("dirLight.ambient", 0.75f, 0.75f, 0.75f);
		lightingShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("dirLight.specular", 0.4f, 0.4f, 0.4f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````sea
		glBindVertexArray(VAOSea);
		glBindTexture(GL_TEXTURE_2D, textureSea);
		model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);

		//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````island
		{
		glBindVertexArray(VAOCrate);
		glBindTexture(GL_TEXTURE_2D, textureIsland);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(ISLAND_X, ISLAND_Y, ISLAND_Z));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 0.8f, 5.0f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		float TREE_X = ISLAND_X + 2.0f;
		float TREE_Z = ISLAND_Z + 1.5f;
		glBindVertexArray(VAO_cc);
		glBindTexture(GL_TEXTURE_2D, textureTree);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(TREE_X, ISLAND_Y, TREE_Z));
		model = glm::rotate(model, glm::radians(10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 1.5f, 0.4f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);

		float leaves_y = CHIMNEY_HEIGHT * 1.5 + ISLAND_Y;
		glBindVertexArray(VAOCrate);
		glBindTexture(GL_TEXTURE_2D, textureLeaves);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(TREE_X - 0.2f, leaves_y - 0.15f, TREE_Z + 0.1f));
		model = glm::rotate(model, glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.1f, 0.1f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(TREE_X - 0.6f, leaves_y - 0.2f, TREE_Z + 0.05f));
		model = glm::rotate(model, glm::radians(-130.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.1f, 0.1f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(TREE_X - 0.55f, leaves_y - 0.15f, TREE_Z - 0.2f));
		model = glm::rotate(model, glm::radians(-80.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.1f, 0.1f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

		//``````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````crates
		lightingShader.setFloat("material.shininess", 16.0f);
		lightingShader.setVec3("material.specular", 0.3f, 0.3f, 0.3f);
		lightingShader.setVec3("dirLight.ambient", 0.45f, 0.45f, 0.45f);
		glBindVertexArray(VAOCrate);
		glBindTexture(GL_TEXTURE_2D, textureCrate);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(LEFT_X + 0.9f*LENGTH + shipMovement, (DECK + FLOOR_HEIGHT) + (CRATE_SIZE*0.3f), MIDDLE_Z + 0.2f*WIDTH));
		model = glm::rotate(model, glm::radians(22.0f), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(LEFT_X + 0.85 * LENGTH + shipMovement, (DECK + FLOOR_HEIGHT) + (CRATE_SIZE*0.3f), MIDDLE_Z - 0.2f*WIDTH));
		model = glm::rotate(model, glm::radians(47.0f), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//``````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````bridge
		lightingShader.setFloat("material.shininess", 16.0f);
		lightingShader.setVec3("material.specular", 0.45f, 0.45f, 0.45f);
		glBindTexture(GL_TEXTURE_2D, textureWall1);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(LEFT_X + 0.3f*LENGTH + shipMovement, (DECK + FLOOR_HEIGHT) + (CRATE_SIZE*BRIDGE_SIZE_Y), MIDDLE_Z + 0.1f*WIDTH));
		model = glm::scale(model, glm::vec3(BRIDGE_SIZE_X, BRIDGE_SIZE_Y, BRIDGE_SIZE_Z));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, textureWall2);
		glDrawArrays(GL_TRIANGLES, 6, 6);
		glBindTexture(GL_TEXTURE_2D, textureWall3);
		glDrawArrays(GL_TRIANGLES, 12, 6);
		glBindTexture(GL_TEXTURE_2D, textureWall4);
		glDrawArrays(GL_TRIANGLES, 18, 6);
		glBindTexture(GL_TEXTURE_2D, textureWall5);
		glDrawArrays(GL_TRIANGLES, 24, 6);

		//``````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````crane
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setVec3("material.specular", 0.55f, 0.55f, 0.55f);
		float crane_z = -0.3f;
		glBindVertexArray(VAOCrane);
		glBindTexture(GL_TEXTURE_2D, textureCrane);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((LEFT_X + 0.65f*LENGTH) + shipMovement, (DECK + FLOOR_HEIGHT) - -0.5f*CRANE_HEIGHT, MIDDLE_Z + crane_z));
		model = glm::scale(model, glm::vec3(CRANE_SIZE, CRANE_HEIGHT, CRANE_SIZE));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((LEFT_X + 0.65f*LENGTH) + 0.7f*0.5f*CRANE_HEIGHT + shipMovement, (DECK + FLOOR_HEIGHT) - (-0.5f - 0.5f)*CRANE_HEIGHT + 0.5f*CRANE_SIZE, MIDDLE_Z + crane_z));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(CRANE_SIZE, CRANE_HEIGHT, CRANE_SIZE));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindTexture(GL_TEXTURE_2D, textureDeck);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((LEFT_X + 0.65f*LENGTH) + 1.5f*0.5f*CRANE_HEIGHT + shipMovement, ((DECK + FLOOR_HEIGHT) - -0.5f * CRANE_HEIGHT) / 0.8, MIDDLE_Z + crane_z));
		model = glm::scale(model, glm::vec3(0.005f, 0.8f*CRANE_HEIGHT, 0.005f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````chimney
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setVec3("material.specular", 0.45f, 0.45f, 0.55f);
		glBindVertexArray(VAO_cc);
		glBindTexture(GL_TEXTURE_2D, textureChimney);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(LEFT_X + 0.1f*LENGTH + shipMovement, (DECK + FLOOR_HEIGHT), MIDDLE_Z - 0.3f*WIDTH));
		lightingShader.setMat4("model", model);
		partBeg = glm::vec3(LEFT_X + 0.1f*LENGTH + shipMovement, (DECK + FLOOR_HEIGHT) + 1.0f*CHIMNEY_HEIGHT, MIDDLE_Z - 0.3f*WIDTH); //smoke beginning
		glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);

		//``````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````barriers
		{
			float diff = abs(WIDTH - DECK_EDGE);
			diff /= BARRIERS_BACK;
			glBindVertexArray(VAO_b);
			glBindTexture(GL_TEXTURE_2D, textureRotator);

			//`````````````````````````````````````````````````````````````````````````````````````````````````````````````````````back
			float d = diff;
			for (int i = 0; i < BARRIERS_BACK; i++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(LEFT_X + DECK_EDGE / 2 + shipMovement, DECK_LEVEL, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2 - d));
				lightingShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
				d += diff;
			}
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + DECK_EDGE / 2 + shipMovement, DECK_LEVEL, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);

			//`````````````````````````````````````````````````````````````````````````````````````````````````````````````````````back_sides
			float diff2 = abs(LEFT_X + DECK_EDGE / 2 - (LEFT_X + 0.5f*LENGTH - ROTATOR_RADIUS));
			diff2 /= BARRIERS_SIDE_BACK;
			d = diff2;
			for (int i = 0; i < BARRIERS_SIDE_BACK; i++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(LEFT_X + DECK_EDGE / 2 + d + shipMovement, DECK_LEVEL, MIDDLE_Z - WIDTH / 2 + DECK_EDGE / 2));
				lightingShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(LEFT_X + DECK_EDGE / 2 + d + shipMovement, DECK_LEVEL, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2));
				lightingShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
				d += diff2;
			}

			//`````````````````````````````````````````````````````````````````````````````````````````````````````````````````````front sides
			float diff3 = abs(LEFT_X + LENGTH - (LEFT_X + 0.5f*LENGTH + ROTATOR_RADIUS));
			diff3 /= BARRIERS_SIDE_FRONT;
			d = diff3;
			for (int i = 0; i < BARRIERS_SIDE_FRONT; i++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(LEFT_X + 0.5f*LENGTH + ROTATOR_RADIUS + d + shipMovement, DECK_LEVEL, MIDDLE_Z - WIDTH / 2 + DECK_EDGE / 2));
				lightingShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(LEFT_X + 0.5f*LENGTH + ROTATOR_RADIUS + d + shipMovement, DECK_LEVEL, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2));
				lightingShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
				d += diff3;
			}
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + 0.5f*LENGTH + ROTATOR_RADIUS + shipMovement, DECK_LEVEL, MIDDLE_Z - WIDTH / 2 + DECK_EDGE / 2));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + 0.5f*LENGTH + ROTATOR_RADIUS + shipMovement, DECK_LEVEL, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + 1.5*LENGTH - 2 * DECK_EDGE + shipMovement, DECK_LEVEL, MIDDLE_Z));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);

			//`````````````````````````````````````````````````````````````````````````````````````````````````````````````````````bukszpryt sides
			float prop;
			float x_prop = abs(LEFT_X + 1.5*LENGTH - 2 * DECK_EDGE - (LEFT_X + LENGTH));
			float z_prop = abs(MIDDLE_Z - (MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2));
			prop = x_prop / z_prop;
			float diff4 = abs(LEFT_X + 1.5*LENGTH - 2 * DECK_EDGE - (LEFT_X + LENGTH));
			diff4 /= BARRIERS_FRONT;
			d = diff4;
			for (int i = 0; i < BARRIERS_FRONT - 1; i++)
			{
				float e = d / prop;
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(LEFT_X + LENGTH + d + shipMovement, DECK_LEVEL, MIDDLE_Z - WIDTH / 2 + DECK_EDGE / 2 + e));
				lightingShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(LEFT_X + LENGTH + d + shipMovement, DECK_LEVEL, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2 - e));
				lightingShader.setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
				d += diff4;
			}

			//`````````````````````````````````````````````````````````````````````````````````````````````````````````````````````barriers_top
			float xxx = abs(WIDTH - DECK_EDGE);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + DECK_EDGE / 2 + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(BARRIER_WIDTH, xxx / BARRIER_HEIGHT, BARRIER_WIDTH));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);

			xxx = abs(LEFT_X + 0.5f*LENGTH - ROTATOR_RADIUS - (LEFT_X + DECK_EDGE / 2));
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + DECK_EDGE / 2 + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(BARRIER_WIDTH, xxx / BARRIER_HEIGHT * BARRIER_LENGTH_FACTOR, BARRIER_WIDTH));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + DECK_EDGE / 2 + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z - WIDTH / 2 + DECK_EDGE / 2));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(BARRIER_WIDTH, xxx / BARRIER_HEIGHT * BARRIER_LENGTH_FACTOR, BARRIER_WIDTH));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);

			xxx = abs(LEFT_X + LENGTH - (LEFT_X + 0.5f*LENGTH + ROTATOR_RADIUS));
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + LENGTH + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(BARRIER_WIDTH, xxx / BARRIER_HEIGHT * BARRIER_LENGTH_FACTOR, BARRIER_WIDTH));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + LENGTH + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z - WIDTH / 2 + DECK_EDGE / 2));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(BARRIER_WIDTH, xxx / BARRIER_HEIGHT * BARRIER_LENGTH_FACTOR, BARRIER_WIDTH));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);

			float fff = glm::atan(prop);
			xxx = abs(LEFT_X + 1.5*LENGTH - 2 * DECK_EDGE - (LEFT_X + LENGTH));
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + LENGTH + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2));
			model = glm::rotate(model, (float)(M_PI_2 - fff), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(BARRIER_WIDTH, xxx / BARRIER_HEIGHT * BARRIER_LENGTH_FACTOR, BARRIER_WIDTH));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + LENGTH + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z - WIDTH / 2 + DECK_EDGE / 2));
			model = glm::rotate(model, (float)-(M_PI_2 - fff), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3(BARRIER_WIDTH, xxx / BARRIER_HEIGHT * BARRIER_LENGTH_FACTOR, BARRIER_WIDTH));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, vertices_cc.size() / 8);


			//`````````````````````````````````````````````````````````````````````````````````````````````````````````````````````barrier_crates
			glBindVertexArray(VAOCrate);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + DECK_EDGE / 2 + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2));
			model = glm::scale(model, glm::vec3(BARRIER_CRATE * BARRIER_RADIUS, BARRIER_CRATE * BARRIER_RADIUS, BARRIER_CRATE * BARRIER_RADIUS));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + DECK_EDGE / 2 + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z - WIDTH / 2 + DECK_EDGE / 2));
			model = glm::scale(model, glm::vec3(BARRIER_CRATE * BARRIER_RADIUS, BARRIER_CRATE * BARRIER_RADIUS, BARRIER_CRATE * BARRIER_RADIUS));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + LENGTH + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z + WIDTH / 2 - DECK_EDGE / 2));
			model = glm::rotate(model, (float)(M_PI_2 - fff) / 2, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(BARRIER_CRATE * BARRIER_RADIUS, BARRIER_CRATE * BARRIER_RADIUS, BARRIER_CRATE * BARRIER_RADIUS * 1.2));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + LENGTH + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z - WIDTH / 2 + DECK_EDGE / 2));
			model = glm::rotate(model, (float)-(M_PI_2 - fff) / 2, glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(BARRIER_CRATE * BARRIER_RADIUS, BARRIER_CRATE * BARRIER_RADIUS, BARRIER_CRATE * BARRIER_RADIUS * 1.2));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(LEFT_X + 1.5*LENGTH - 2 * DECK_EDGE + shipMovement, DECK_LEVEL + BARRIER_HEIGHT, MIDDLE_Z));
			model = glm::scale(model, glm::vec3(BARRIER_CRATE * BARRIER_RADIUS * 1.5, BARRIER_CRATE * BARRIER_RADIUS, BARRIER_CRATE * BARRIER_RADIUS * 2));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````rotators
		lightingShader.setFloat("material.shininess", 16.0f);
		lightingShader.setVec3("material.specular", 0.65f, 0.65f, 0.65f);
		glBindVertexArray(VAO_rr);
		glBindTexture(GL_TEXTURE_2D, textureRotator);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(LEFT_X + 0.5f*LENGTH + shipMovement, DECK_LEVEL - DECK_HEIGHT, (WIDTH / 2) - 0.4f*ROTATOR_WIDTH));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_rr.size() / 8);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(LEFT_X + 0.5f*LENGTH + shipMovement, DECK_LEVEL - DECK_HEIGHT, (-WIDTH / 2) - 0.6f*ROTATOR_WIDTH));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_rr.size() / 8);

		//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````paddels
		lightingShader.setFloat("material.shininess", 16.0f);
		lightingShader.setVec3("material.specular", 0.35f, 0.35f, 0.35f);
		glBindVertexArray(VAO_pp);
		glBindTexture(GL_TEXTURE_2D, texturePaddles);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(LEFT_X + 0.5f*LENGTH + shipMovement, DECK_LEVEL - 0.7f*DECK_HEIGHT, (WIDTH / 2)));
		model = glm::rotate(model, glm::radians(paddle_angle), glm::vec3(0.0, 0.0, 1.0));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_pp.size() / 8);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -WIDTH - ROTATOR_WIDTH_2));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_pp.size() / 8);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(LEFT_X + 0.5f*LENGTH + shipMovement, DECK_LEVEL - 0.7f*DECK_HEIGHT, (WIDTH / 2)));
		model = glm::rotate(model, glm::radians(31.5f + paddle_angle), glm::vec3(0.0, 0.0, 1.0));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_pp.size() / 8);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -WIDTH - ROTATOR_WIDTH_2));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_pp.size() / 8);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(LEFT_X + 0.5f*LENGTH + shipMovement, DECK_LEVEL - 0.7f*DECK_HEIGHT, (WIDTH / 2)));
		model = glm::rotate(model, glm::radians(60.5f + paddle_angle), glm::vec3(0.0, 0.0, 1.0));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_pp.size() / 8);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -WIDTH - ROTATOR_WIDTH_2));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_pp.size() / 8);

		//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````circle rotators
		lightingShader.setFloat("material.shininess", 16.0f);
		lightingShader.setVec3("material.specular", 0.75f, 0.75f, 0.75f);
		glBindVertexArray(VAO_circle);
		glBindTexture(GL_TEXTURE_2D, textureCircle);
		model = glm::mat4(1.0f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		model = glm::translate(model, glm::vec3(LEFT_X + 0.5f*LENGTH + shipMovement, DECK_LEVEL - 0.7f*DECK_HEIGHT, (WIDTH / 2) + ROTATOR_WIDTH_2 + 0.001f));
		model = glm::rotate(model, glm::radians(paddle_angle), glm::vec3(0.0, 0.0, 1.0));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_circle.size() / 8);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -WIDTH - 2 * ROTATOR_WIDTH_2 - 0.002f));
		model = glm::rotate(model, glm::radians(paddle_angle), glm::vec3(0.0, 0.0, 1.0));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, vertices_circle.size() / 8);

		//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````main board
		shaderSea.use();
		shaderSea.setMat4("projection", projection);
		shaderSea.setMat4("view", view);
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, textureBottom);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(shipMovement, 0.0f, 0.0f));
		shaderSea.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, bottom.size() / 5);
		glBindVertexArray(VAODeck);
		glBindTexture(GL_TEXTURE_2D, textureDeck);
		glDrawArrays(GL_TRIANGLES, 0, deck.size() / 5);

		//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````floor
		lightingShader.use();
		lightingShader.setFloat("material.shininess", 256.0f);
		lightingShader.setVec3("material.specular", 0.70f, 0.70f, 0.70f);
		lightingShader.setMat4("model", model);
		glBindVertexArray(VAOFloor);
		glBindTexture(GL_TEXTURE_2D, textureFloor);
		glDrawArrays(GL_TRIANGLES, 0, floor.size() / 8);

		//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````smoke
		particleShader.use();
		particleShader.setMat4("projection", projection);
		particleShader.setMat4("view", view);
		if (smokeSpeed != 0.0f)
		{
			particle.updateParticles(0.01f, smokeSpeed, stand);
			for (Particle p : particle.particles)
			{
				if (p.getLife() > 0.0f)
				{
					glBindVertexArray(VAOParticle);
					model = glm::mat4(1.0f);
					model = glm::translate(model, partBeg);
					model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
					particleShader.setMat4("model", model);
					particleShader.setVec3("offset", p.getPosition());
					particleShader.setVec4("color", p.getColor());
					glDrawArrays(GL_TRIANGLES, 0, 36);
					glBindVertexArray(0);
				}
			}
		}

		//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````circle chimney bottom
		glBindVertexArray(VAO_circle_c);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(LEFT_X + 0.1f*LENGTH + shipMovement, (DECK + FLOOR_HEIGHT) + 0.01f, MIDDLE_Z - 0.3f*WIDTH));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
		particleShader.setMat4("model", model);
		particleShader.setVec3("offset", glm::vec3(0.0f, 0.0f, 0.0f));
		particleShader.setVec4("color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, vertices_circle_c.size() / 3);

		//````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````````skybox cube
		glDepthFunc(GL_LEQUAL);
		skyboxShader.use();
		view = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp)));
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (forward)
		{
			if (acceleration < 0.05f) {
				acceleration += 0.003f;
				smokeSpeed += acceleration;
			}
		}
		else
		{
			forward = true;
			backward = stand = false;
			acceleration = 0.002f;
			smokeSpeed = 0.02f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (backward)
		{
			if (acceleration < 0.05f) {
				acceleration += 0.003f;
				smokeSpeed -= acceleration;
			}
		}
		else
		{
			backward = true;
			forward = stand = false;
			acceleration = 0.002f;
			smokeSpeed = -0.02f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		stand = true;
		forward = backward = false;
		acceleration = 0.002f;
		smokeSpeed = 0.01f;
	}
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}
unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	stbi_set_flip_vertically_on_load(false);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	return textureID;
}
void normalize(glm::vec3 &v)
{
	float magnitude = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
	v.x = v.x / magnitude;
	v.y = v.y / magnitude;
	v.z = v.z / magnitude;
}
void generateTex(unsigned int texture, const char* path)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

