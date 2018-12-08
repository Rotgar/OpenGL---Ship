#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#include <headers/stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <headers/Shader.hpp>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#define DECK_LEVEL -0.4f
#define DECK_HEIGHT 0.25f
#define BOTTOM_LEVEL -0.95f
#define LEFT_X -0.8f
#define LEFT_Z 0.45f
#define LENGTH 1.5f
#define WIDTH 0.9f

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

std::vector<glm::vec3> draw_cylinder_v(float x, float y, float z, float r1, float r2, float r3, int o, int nr_points);
std::vector<unsigned int> draw_cylinder_i(int nr_points, int o);

std::vector<glm::vec3> draw_cuboid_v(float x, float y, float z, float length, float width, float height);
std::vector<unsigned int> draw_cuboid_i();

std::vector<glm::vec3> draw_bottom(glm::vec3 left, glm::vec3 right, glm::vec3 left_up, int nr_points);
std::vector<unsigned int> draw_bottom_i(int nr_points);
std::vector<glm::vec3> draw_deck(glm::vec3 bukszpryt_middle, glm::vec3 bukszpryt_down);
std::vector<unsigned int> draw_deck_i(int nr_points);

std::vector<glm::vec3> draw_rotator_v(float x, float y, float z, float r1, float r2, float width, int nr_points);
std::vector<unsigned int> draw_rotator_i(int nr_points);

std::vector<glm::vec3> draw_paddles_v(float x, float y, float z, float r1, float width, float length);
std::vector<unsigned int> draw_paddles_i();

std::vector<glm::vec3> draw_circle_v(float x, float y, float z, float r, int nr_points);
std::vector<unsigned int> draw_circle_i(int nr_points);


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
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
float fov = 45.0f;

float currentX = 0.0f, currentZ = 0.0f;


float shipMovement = 0.0f, acceleration = 0.005f, rotation = 0.0f;
bool stand = true, forward = false , backward = false, left = false, right = false;



// lighting
glm::vec3 lightPos(1.2f, 2.0f, 5.0f);


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
	
	Shader ourShader("Shaders\\shader.vs", "Shaders\\shader.fs"),
		cylinder("Shaders\\shader_c.vs", "Shaders\\shader_c.fs");

	glm::vec3 left_bottom = { LEFT_X, BOTTOM_LEVEL, 0.0f };
	glm::vec3 right_bottom = { LEFT_X + LENGTH, BOTTOM_LEVEL, 0.0f };
	glm::vec3 left_up = { LEFT_X, DECK_LEVEL - DECK_HEIGHT, LEFT_Z };
	std::vector<glm::vec3> bottom = draw_bottom(left_bottom, right_bottom, left_up, 16);
	std::vector<unsigned> bottom_i = draw_bottom_i(bottom.size());

	glm::vec3 bukszpryt = { LEFT_X + 1.2*LENGTH, DECK_LEVEL - DECK_HEIGHT, 0.0f };
	std::vector<glm::vec3> deck = draw_deck(bukszpryt, right_bottom);
	std::vector<unsigned> deck_i = draw_deck_i(deck.size());

	float verticesCrane[] = {

		//dzwig
		 0.22f, -0.4f,  0.0f,  1.0f, 0.0f, 0.0f,	// bottom left	front	  0
		 0.32f, -0.4f,  0.0f,  1.0f, 0.0f, 0.0f,	// bottom right	front	  1
		 0.22f, -0.4f, -0.1f,  1.0f, 0.0f, 0.0f,	// bottom left	behind	  2
		 0.32f, -0.4f, -0.1f,  1.0f, 0.0f, 0.0f,	// bottom right	behind	  3

		 0.4f,  0.1f, -0.02f,  1.0f, 0.0f, 0.0f,	// top left	 front		  4
		 0.5f,  0.1f, -0.02f,  1.0f, 0.0f, 0.0f,	// top right front		  5
		 0.4f,  0.1f, -0.08f,  1.0f, 0.0f, 0.0f,	// top left	 behind		  6
		 0.5f,  0.1f, -0.08f,  1.0f, 0.0f, 0.0f,	// top right behind		  7

		 0.48f,  0.05f, -0.02f,  1.0f, 0.0f, 0.0f,  // bottom left  front	  8
		 0.9f ,  0.1f,  -0.05f,  1.0f, 0.0f, 0.0f,  // bottom right front	  9
		 0.48f,  0.05f, -0.08f,  1.0f, 0.0f, 0.0f   // bottom left  behind     10
	};

	unsigned int indicesCrane[] = {
	
		//dzwig 16
		0, 1, 2,
		1, 2, 3,
		0, 1, 4,
		1, 4, 5,
		0, 2, 4,
		2, 4, 6,
		2, 3, 6,
		3, 6, 7,
		1, 3, 5,
		3, 5, 7,
		4, 5, 6,
		5, 6, 7,
		5, 7, 9,
		8, 5, 9,
		9, 10, 7,
		10, 8, 9
	};

	
	unsigned int VAO, VBO, EBO;
	unsigned int VAOChimney, VBOChimney, EBOChimney;
	unsigned int VAOCrane, VBOCrane, EBOCrane;
	unsigned int VAODeck, VBODeck, EBODeck;


	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAOChimney);
	glGenVertexArrays(1, &VAOCrane);
	glGenVertexArrays(1, &VAODeck);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBOChimney);
	glGenBuffers(1, &VBOCrane);
	glGenBuffers(1, &VBODeck);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &EBOChimney);
	glGenBuffers(1, &EBOCrane);
	glGenBuffers(1, &EBODeck);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*bottom.size(), &bottom[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*bottom_i.size(), &bottom_i[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//main part - deck
	glBindVertexArray(VAODeck);
	glBindBuffer(GL_ARRAY_BUFFER, VBODeck);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*deck.size(), &deck[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBODeck);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*deck_i.size(), &deck_i[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	// Crane
	glBindVertexArray(VAOCrane);

	glBindBuffer(GL_ARRAY_BUFFER, VBOCrane);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCrane), verticesCrane, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOCrane);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCrane), indicesCrane, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	//cylinder chimney - _c
	int nr_points = 60;
	std::vector<glm::vec3> vertices_c = draw_cylinder_v(0, 1.2, 0, 0.17, 0.10, 0.06, 2, nr_points);
	std::vector<unsigned int> indices_c = draw_cylinder_i(nr_points, 2);

	unsigned int VAO_c, VBO_c, EBO_c;
	glGenVertexArrays(1, &VAO_c);
	glGenBuffers(1, &VBO_c);
	glGenBuffers(1, &EBO_c);

	glBindVertexArray(VAO_c);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_c);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices_c.size() , &vertices_c[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_c);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices_c.size(), &indices_c[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//rotator - r

	float RX = -0.2f, RY = -0.85f, RZ = 1.0f, R1 = 0.4f, R2 = 0.65f, length = -0.4f;

	nr_points = 10;
	std::vector<glm::vec3> vertices_r = draw_rotator_v(RX, RY, RZ, R1, R2, length, nr_points);
	std::vector<unsigned int> indices_r = draw_rotator_i(nr_points);

	unsigned int VAO_r, VBO_r, EBO_r;
	glGenVertexArrays(1, &VAO_r);
	glGenBuffers(1, &VBO_r);
	glGenBuffers(1, &EBO_r);

	glBindVertexArray(VAO_r);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_r);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices_r.size(), &vertices_r[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_r);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices_r.size(), &indices_r[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// paddles

	float p_width = 0.08f;
	std::vector<glm::vec3> vertices_p = draw_paddles_v(RX, RY, RZ, R1 + 0.03f, p_width, length+0.2f);
	std::vector<unsigned int> indices_p = draw_paddles_i();

	unsigned int VAO_p, VBO_p, EBO_p;
	glGenVertexArrays(1, &VAO_p);
	glGenBuffers(1, &VBO_p);
	glGenBuffers(1, &EBO_p);

	glBindVertexArray(VAO_p);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_p);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices_p.size(), &vertices_p[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_p);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices_p.size(), &indices_p[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	// circle
	nr_points = 20;
	std::vector<glm::vec3> vertices_circle = draw_circle_v(RX, RY, RZ, 0.2f, nr_points);
	std::vector<unsigned int> indices_circle = draw_circle_i(nr_points);

	unsigned int VAO_circle, VBO_circle, EBO_circle;
	glGenVertexArrays(1, &VAO_circle);
	glGenBuffers(1, &VBO_circle);
	glGenBuffers(1, &EBO_circle);

	glBindVertexArray(VAO_circle);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_circle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices_circle.size(), &vertices_circle[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_circle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices_circle.size(), &indices_circle[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);










	// cuboid------------------
	std::vector<glm::vec3> vertices_c1 = draw_cuboid_v(0, 0, 0, 0.4f, 0.3f, 0.3f);
	std::vector<unsigned int> indices_c1 = draw_cuboid_i();

	unsigned int VAO_c1, VBO_c1, EBO_c1;
	glGenVertexArrays(1, &VAO_c1);
	glGenBuffers(1, &VBO_c1);
	glGenBuffers(1, &EBO_c1);

	glBindVertexArray(VAO_c1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_c1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices_c1.size(), &vertices_c1[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_c1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indices_c1.size(), &indices_c1[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);





	

//``````````````````````````Sea
	
	Shader shaderSea("Shaders\\shaderTex.vs", "Shaders\\shaderTex.fs");

	unsigned int VAOSea, VBOSea, EBOSea;
	glGenVertexArrays(1, &VAOSea);
	glGenBuffers(1, &VBOSea);
	glGenBuffers(1, &EBOSea);

	glBindVertexArray(VAOSea);

	float verticesSea[] = {
		// positions          // colors           // texture coords
		 1000.0f, -1.1f,  1000.0f,   300.0f, 300.0f, // top right
		 1000.0f, -1.1f, -1000.0f,   300.0f, 0.0f, // bottom right
		-1000.0f, -1.1f, -1000.0f,   0.0f, 0.0f, // bottom left
		-1000.0f, -1.1f,  1000.0f,   0.0f, 300.0f  // top left 
	};
	unsigned int indicesSea[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	glBindBuffer(GL_ARRAY_BUFFER, VBOSea);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSea), verticesSea, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOSea);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSea), indicesSea, GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char *data = stbi_load("Pictures\\sea2.jpg", &width, &height, &nrChannels, 0);
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
	
	//shaderSea.use();
	//glUniform1i(glGetUniformLocation(shaderSea.ID, "texture1"), 0);



	// 

	float verticesCrate[] = {
		//positions           //normals            //texture
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	unsigned int VBOCrate, VAOCrate;
	glGenVertexArrays(1, &VAOCrate);
	glGenBuffers(1, &VBOCrate);

	glBindVertexArray(VAOCrate);

	glBindBuffer(GL_ARRAY_BUFFER, VBOCrate);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCrate), verticesCrate, GL_STATIC_DRAW);

	glBindVertexArray(VAOCrate);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	unsigned int textureCrate;

	glGenTextures(1, &textureCrate);
	glBindTexture(GL_TEXTURE_2D, textureCrate);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load("Pictures\\crate.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);


	Shader lightingShader("Shaders\\1.colors.vs", "Shaders\\1.colors.fs");
	Shader lampShader("Shaders\\1.lamp.vs", "Shaders\\1.lamp.fs");

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	// we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
	glBindBuffer(GL_ARRAY_BUFFER, VBOCrate);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);




	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);













	float paddle_angle = 0.0f;

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
				paddle_angle -= 10 * acceleration + 1.0f;
			}
			else if (backward)
			{
				shipMovement -= acceleration;
				paddle_angle += 10 * acceleration+1.0f;
			}
		}

		glClearColor(0.45f, 0.8f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

		ourShader.use();
		unsigned int colorLoc = glGetUniformLocation(ourShader.ID, "newColor");

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);

		
		// sea
		glBindTexture(GL_TEXTURE_2D, texture);
		shaderSea.use();
		glBindVertexArray(VAOSea);
		model = glm::mat4(1.0f);
		shaderSea.setMat4("model", model);
		shaderSea.setMat4("view", view);
		shaderSea.setMat4("projection", projection);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		lightingShader.use();
		lightingShader.setVec3("viewPos", cameraPos);
		lightingShader.setFloat("material.shininess", 32.0f);
		lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("dirLight.direction", -1.2f, -1.0f, 0.0f);
		lightingShader.setVec3("dirLight.ambient", 0.15f, 0.15f, 0.1f);
		lightingShader.setVec3("dirLight.diffuse", 0.3f, 0.3f, 0.3f);
		lightingShader.setVec3("dirLight.specular", 0.7f, 0.7f, 0.7f);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);


		// crates
		glBindVertexArray(VAOCrate);
		glBindTexture(GL_TEXTURE_2D, textureCrate);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.2 + shipMovement, -0.45, 0.3));
		model = glm::rotate(model, glm::radians(22.0f), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.2 + shipMovement, -0.45, -0.3));
		model = glm::rotate(model, glm::radians(47.0f), glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// also draw the lamp object
		/*
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		*/



		// bridge
		ourShader.use();
		glBindVertexArray(VAO_c1);
		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-1.0 + shipMovement, -0.6, 0));
		model = glm::scale(model, glm::vec3(2.5f, 1.6f, 1.8f));
		shaderSea.setMat4("model", model);
		glUniform4f(colorLoc, 0.0, 1.0, 0.0, 1.0);
		glDrawElements(GL_TRIANGLES, indices_c1.size(), GL_UNSIGNED_INT, 0);



		// main board
		ourShader.use();

		glBindVertexArray(VAO);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(shipMovement, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.5f, 1.5f));
		ourShader.setMat4("model", model);
		glUniform4f(colorLoc, 0.5, 0.3, 0.1, 1.0);
		glDrawElements(GL_TRIANGLES, bottom_i.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(VAODeck);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(shipMovement, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.5f, 1.5f));
		ourShader.setMat4("model", model);
		glUniform4f(colorLoc, 0.5, 0.3, 0.1, 1.0);
		glDrawElements(GL_TRIANGLES, deck_i.size(), GL_UNSIGNED_INT, 0);



		//crane
		glBindVertexArray(VAOCrane);
		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(shipMovement, 0.1, 0));
	
		model = glm::scale(model, glm::vec3(1.5f, 2.0f, 0.8f));
		ourShader.setMat4("model", model);

		glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);
		glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);




		// chimney
		cylinder.use();
		unsigned int cylinder_colorLoc = glGetUniformLocation(cylinder.ID, "newColor");

		glBindVertexArray(VAO_c);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.4 + shipMovement, -0.6, -0.3));
	
		glUniform4f(cylinder_colorLoc, 1.0, 0.0, 0.0, 1.0);
		cylinder.setMat4("projection", projection);
		cylinder.setMat4("view", view);
		cylinder.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices_c.size(), GL_UNSIGNED_INT, 0);
	

		glm::mat4 m1, m2, m3;

		// rotator
		glBindVertexArray(VAO_r);
		model = glm::mat4(1.0f);

		glUniform4f(cylinder_colorLoc, 1.0, 1.0, 0.0, 1.0);
		cylinder.setMat4("projection", projection);
		cylinder.setMat4("view", view);
		model = glm::translate(model, glm::vec3(shipMovement, 0.0f, 0.0f));
		cylinder.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices_r.size(), GL_UNSIGNED_INT, 0);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(shipMovement, 0.0f, RZ - 2.6f)),
		cylinder.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices_r.size(), GL_UNSIGNED_INT, 0);



		// paddles
		glBindVertexArray(VAO_p);
		model = glm::mat4(1.0f);

		glUniform4f(cylinder_colorLoc, 1.0, 0.0, 1.0, 1.0);
		cylinder.setMat4("projection", projection);
		cylinder.setMat4("view", view);
		m1 = glm::translate(glm::mat4(1.0f), glm::vec3(RX + shipMovement, RY, RZ)),
		m2 = glm::rotate(glm::mat4(1.0f), glm::radians(paddle_angle), glm::vec3(0.0, 0.0, 1.0)),
		m3 = glm::translate(glm::mat4(1.0f), glm::vec3(-RX, -RY, -RZ));
		model = m1 * m2 * m3;
		cylinder.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices_p.size(), GL_UNSIGNED_INT, 0);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, RZ - 2.78f)),
		cylinder.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices_p.size(), GL_UNSIGNED_INT, 0);

		m1 = glm::translate(glm::mat4(1.0f), glm::vec3(RX + shipMovement, RY, RZ)),
		m2 = glm::rotate(glm::mat4(1.0f), glm::radians(31.5f + paddle_angle), glm::vec3(0.0, 0.0, 1.0)),
		m3 = glm::translate(glm::mat4(1.0f), glm::vec3(-RX, -RY, -RZ));
		model = m1 * m2 * m3;

		cylinder.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices_p.size(), GL_UNSIGNED_INT, 0);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, RZ - 2.78f));
		cylinder.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices_p.size(), GL_UNSIGNED_INT, 0);

		m1 = glm::translate(glm::mat4(1.0f), glm::vec3(RX + shipMovement, RY, RZ)),
		m2 = glm::rotate(glm::mat4(1.0f), glm::radians(60.5f + paddle_angle), glm::vec3(0.0, 0.0, 1.0)),
		m3 = glm::translate(glm::mat4(1.0f), glm::vec3(-RX, -RY, -RZ));
		model = m1 * m2 * m3;

		cylinder.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices_p.size(), GL_UNSIGNED_INT, 0);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, RZ - 2.78f));
		cylinder.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices_p.size(), GL_UNSIGNED_INT, 0);



		// circle
		glBindVertexArray(VAO_circle);
		model = glm::mat4(1.0f);

		glUniform4f(cylinder_colorLoc, 0.0, 1.0, 1.0, 1.0);
		cylinder.setMat4("projection", projection);
		cylinder.setMat4("view", view);
		
		model = glm::translate(model, glm::vec3(shipMovement, 0.0f, 0.0f));
		cylinder.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices_circle.size(), GL_UNSIGNED_INT, 0);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, RZ - 2.983f));
		cylinder.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, indices_p.size(), GL_UNSIGNED_INT, 0);




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
			if (acceleration < 0.05f)
				acceleration += 0.003f;

			//currentX = acceleration;
		}
		else 
		{
			forward = true;
			backward = stand = false;
			acceleration = 0.002f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (backward)
		{
			if (acceleration < 0.05f)
				acceleration += 0.003f;
		}
		else
		{
			backward = true;
			forward = stand = false;
			acceleration = 0.002f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		stand = true;
		forward = backward = false;
		acceleration = 0.002f;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		rotation += 1.0f;
		currentX += 0.01 *glm::cos(glm::radians(rotation));
		currentZ -= 0.01 *glm::sin(glm::radians(rotation));

		left = true;
		right = false;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		rotation -= 1.0f;
		currentX += 0.01 * glm::cos(glm::radians(rotation));
		currentZ -= 0.01 * glm::sin(glm::radians(rotation));

		right = true;
		left = false;
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


// o == 0 - normal cylinder, o == 1 - cylinder with lower r up, o == 2 - cylinder without insides
std::vector<glm::vec3> draw_cylinder_v(float x, float y, float z, float r1, float r2, float r3, int o, int nr_points)
{
	std::vector<glm::vec3> vertices;
	if (o != 2)
	{
		vertices.push_back(glm::vec3(x, 0, z));
		for (float i = 1; i <= nr_points; ++i)
		{
			float u = i / (float)nr_points,
				nx = x + r1 * cos(u * 2.0f*M_PI),
				nz = z + r1 * sin(u * 2.0f*M_PI);

			vertices.push_back(glm::vec3(nx, 0, nz));
		}
		vertices.push_back(glm::vec3(x, y, z));

		float r = r1;
		if (o != 0)
		{
			r = r2;
		}

		for (float i = 1; i <= nr_points; ++i)
		{
			float u = i / (float)nr_points,
				nx = x + r * cos(u * 2.0f*M_PI),
				nz = z + r * sin(u * 2.0f*M_PI);

			vertices.push_back(glm::vec3(nx, y, nz));
		}
	}
	else
	{
		for (float i = 1; i <= nr_points; ++i)
		{
			float u = i / (float)nr_points,
				nx = x + r1 * cos(u * 2.0f*M_PI),
				nz = z + r1 * sin(u * 2.0f*M_PI);

			vertices.push_back(glm::vec3(nx, 0, nz));
		}

		for (float i = 1; i <= nr_points; ++i)
		{
			float u = i / (float)nr_points,
				nx = x + r3 * cos(u * 2.0f*M_PI),
				nz = z + r3 * sin(u * 2.0f*M_PI);

			vertices.push_back(glm::vec3(nx, 0, nz));
		}

		for (float i = 1; i <= nr_points; ++i)
		{
			float u = i / (float)nr_points,
				nx = x + r2 * cos(u * 2.0f*M_PI),
				nz = z + r2 * sin(u * 2.0f*M_PI);

			vertices.push_back(glm::vec3(nx, y, nz));
		}

		for (float i = 1; i <= nr_points; ++i)
		{
			float u = i / (float)nr_points,
				nx = x + r3 * cos(u * 2.0f*M_PI),
				nz = z + r3 * sin(u * 2.0f*M_PI);

			vertices.push_back(glm::vec3(nx, y, nz));
		}
	}
	return vertices;
}
std::vector<unsigned int> draw_cylinder_i(int nr_points, int o)
{
	std::vector<unsigned int> indices;

	if (o != 2)
	{
		for (int i = 1; i < nr_points; ++i)
		{
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i + 1);
		}
		indices.push_back(0);
		indices.push_back(nr_points);
		indices.push_back(1);

		for (int i = nr_points + 2; i < 2 * nr_points + 1; ++i)
		{
			indices.push_back(nr_points + 1);
			indices.push_back(i);
			indices.push_back(i + 1);
		}
		indices.push_back(nr_points + 1);
		indices.push_back(2 * nr_points + 1);
		indices.push_back(nr_points + 2);

		for (int i = 1; i < nr_points; ++i)
		{
			indices.push_back(i);
			indices.push_back(i + 1);
			indices.push_back(nr_points + 1 + i);

			indices.push_back(i + 1);
			indices.push_back(nr_points + 1 + i);
			indices.push_back(nr_points + 2 + i);
		}
		indices.push_back(nr_points);
		indices.push_back(1);
		indices.push_back(2 * nr_points + 1);

		indices.push_back(1);
		indices.push_back(2 * nr_points + 1);
		indices.push_back(nr_points + 2);
	}

	else
	{
		for (int i = 0; i < nr_points; ++i)
		{
			indices.push_back(i);
			indices.push_back((i + 1)%nr_points);
			indices.push_back(i + nr_points);

			if (i == 0)
				indices.push_back(2 * nr_points - 1);
			else
				indices.push_back(i + nr_points - 1);
			
			indices.push_back(i);
			indices.push_back(i + nr_points);
		}

		for (int i = 2 * nr_points; i < 3 * nr_points; ++i)
		{
			indices.push_back(i);
			indices.push_back((i + 1) % nr_points + 2 * nr_points);
			indices.push_back(i + nr_points);

			if (i == 2 * nr_points)
				indices.push_back(4 * nr_points - 1);
			else
				indices.push_back(i + nr_points - 1);

			indices.push_back(i);
			indices.push_back(i + nr_points);
		}

		for (int i = 0; i < nr_points; ++i)
		{
			indices.push_back(i);
			indices.push_back((i + 1) % nr_points);
			indices.push_back(i + 2 * nr_points);

			indices.push_back((i + 1) % nr_points);
			indices.push_back((i + 1) % nr_points + 2 * nr_points);
			indices.push_back(i + 2 * nr_points);
		}

		for (int i = nr_points; i < 2 * nr_points; ++i)
		{
			indices.push_back(i);
			indices.push_back((i + 1) % nr_points + nr_points);
			indices.push_back(i + 2 * nr_points);

			indices.push_back((i + 1) % nr_points + nr_points);
			indices.push_back((i + 1) % nr_points + nr_points + 2 * nr_points);
			indices.push_back(i + 2 * nr_points);
		}
	}
	return indices;
}
std::vector<glm::vec3> draw_cuboid_v(float x, float y, float z, float length, float width, float height)
{
	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(x, y, z));
	vertices.push_back(glm::vec3(x + length, y, z));
	vertices.push_back(glm::vec3(x + length, y, z + width));
	vertices.push_back(glm::vec3(x, y, z + width));
	vertices.push_back(glm::vec3(x, y + height, z));
	vertices.push_back(glm::vec3(x + length, y + height, z));
	vertices.push_back(glm::vec3(x + length, y + height, z + width));
	vertices.push_back(glm::vec3(x, y + height, z + width));

	return vertices;
}
std::vector<unsigned int> draw_cuboid_i()
{
	std::vector<unsigned int> indices;

	for (int i = 0; i < 4; ++i)
	{
		indices.push_back(i);
		indices.push_back((i+1) % 4);
		indices.push_back(i+4);

		indices.push_back((i+1) % 4);
		indices.push_back((i+5)%4 + 4);
		indices.push_back(i+4);
	}
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(7);

	indices.push_back(5);
	indices.push_back(7);
	indices.push_back(6);


	return indices;
}
//~````````````````````````````````````````````````Main part
std::vector<glm::vec3> draw_bottom(glm::vec3 left, glm::vec3 right, glm::vec3 left_up, int nr_points)
{
	std::vector<glm::vec3> result;
	result.push_back(left);
	result.push_back(right);
	//some maths here - computing the parabola equation 
	//
	float z = left[2];
	float y = left[1];
	float z2 = left_up[2];
	float y2 = left_up[1];
	float a = (y2 - y) / (z2*z2 - 2 * z2*z + z * z);
	float b = -2 * z*a;
	float c = y + z * z*a;
	float step = (z2 - z) / nr_points;
	float step_in_x = (left_up[0] - left[0]) / nr_points;
	float new_x = left[0];
	float new_rx = right[0];
	for (int i = 1; i < nr_points; i++)
	{
		new_x += step_in_x;
		new_rx += step_in_x;
		z += step;
		y = a * z*z + b * z + c;
		glm::vec3 new_vert_left = { new_x, y, z };
		result.push_back(new_vert_left);
		glm::vec3 new_vert_right = { new_rx, y, z };
		result.push_back(new_vert_right);
	}
	glm::vec3 right_up = { new_rx + step_in_x, left_up[1], left_up[2] };
	result.push_back(left_up);
	result.push_back(right_up);
	//now we have to generate mirrored part
	new_x = left[0];
	new_rx = right[0];
	z = left[2];
	y = left[1];
	for (int i = 1; i < nr_points; i++)
	{
		new_x += step_in_x;
		new_rx += step_in_x;
		z -= step;
		y = a * z*z + b * z + c;
		glm::vec3 new_vert_left = { new_x, y, z };
		result.push_back(new_vert_left);
		glm::vec3 new_vert_right = { new_rx, y, z };
		result.push_back(new_vert_right);
	}
	glm::vec3 right_up2 = { new_rx + step_in_x, left_up[1], left_up[2] - 2 * (left_up[2] - left[2]) };
	glm::vec3 left_up2 = { new_x + step_in_x, left_up[1], left_up[2] - 2 * (left_up[2] - left[2]) };
	//corrections
	result.push_back(left_up2);
	result.push_back(right_up2);
	glm::vec3 left_middle = { left[0], left_up[1], left[2] };
	glm::vec3 right_middle = { right[0], left_up[1], left[2] };
	result.push_back(left_middle);
	result.push_back(right_middle);
	glm::vec3 bukszpryt = { right[0] + 0.2*LENGTH, right_middle[1], right_middle[2] };
	result.push_back(bukszpryt);
	result.push_back(bukszpryt); //doubled to keep the parity
	return result;
}
std::vector<unsigned int> draw_bottom_i(int nr_points)
{
	std::vector<unsigned int> result;
	for (int i = 0; i < nr_points / 2 - 3; i++)
	{
		result.push_back(i);
		result.push_back(i + 1);
		result.push_back(i + 2);

		if (i % 2)
		{
			result.push_back(i);
			result.push_back(i + 2);
			result.push_back(nr_points - 3);
			result.push_back(i);
			result.push_back(i + 2);
			result.push_back(nr_points - 2);
		}
		else
		{
			result.push_back(i);
			result.push_back(i + 2);
			result.push_back(nr_points - 4);
		}
	}
	result.push_back(0);
	result.push_back(1);
	result.push_back(nr_points / 2 - 1);
	result.push_back(1);
	result.push_back(nr_points / 2 - 1);
	result.push_back(nr_points / 2);
	result.push_back(0);
	result.push_back(nr_points / 2 - 1);
	result.push_back(nr_points - 4);
	result.push_back(1);
	result.push_back(nr_points / 2);
	result.push_back(nr_points - 3);
	for (int i = nr_points / 2 - 1; i < nr_points - 6; i++)
	{
		result.push_back(i);
		result.push_back(i + 1);
		result.push_back(i + 2);
		if (i % 2)
		{
			result.push_back(i);
			result.push_back(i + 2);
			result.push_back(nr_points - 3);
			result.push_back(i);
			result.push_back(i + 2);
			result.push_back(nr_points - 2);
		}
		else
		{
			result.push_back(i);
			result.push_back(i + 2);
			result.push_back(nr_points - 4);
		}
	}
	result.push_back(1);
	result.push_back(nr_points / 2);
	result.push_back(nr_points - 2);
	return result;
}
std::vector<glm::vec3> draw_deck(glm::vec3 bukszpryt, glm::vec3 bukszpryt_down)
{
	float proportion = abs(LEFT_X - LEFT_X + LENGTH) / 100;
	std::vector<glm::vec3> result;
	glm::vec3 v0 = { LEFT_X, DECK_LEVEL - DECK_HEIGHT, LEFT_Z };
	result.push_back(v0);
	glm::vec3 v1 = { LEFT_X, DECK_LEVEL - DECK_HEIGHT, -LEFT_Z };
	result.push_back(v1);
	glm::vec3 v2 = { LEFT_X, DECK_LEVEL, LEFT_Z };
	result.push_back(v2);
	glm::vec3 v3 = { LEFT_X, DECK_LEVEL, -LEFT_Z };
	result.push_back(v3);
	glm::vec3 v4 = { LEFT_X + LENGTH, DECK_LEVEL - DECK_HEIGHT, -LEFT_Z };
	result.push_back(v4);
	glm::vec3 v5 = { LEFT_X + LENGTH, DECK_LEVEL - DECK_HEIGHT, LEFT_Z };
	result.push_back(v5);
	glm::vec3 v6 = { LEFT_X + LENGTH, DECK_LEVEL, LEFT_Z };
	result.push_back(v6);
	glm::vec3 v7 = { LEFT_X + LENGTH, DECK_LEVEL, -LEFT_Z };
	result.push_back(v7);
	result.push_back(bukszpryt);
	float dx = (bukszpryt[0] - bukszpryt_down[0])*((DECK_LEVEL - bukszpryt[1]) / (bukszpryt[1] - bukszpryt_down[1]));
	glm::vec3 v8 = { bukszpryt[0] + dx, DECK_LEVEL, bukszpryt[2] };
	result.push_back(v8);
	float x1 = LEFT_X;
	float z1 = (LEFT_Z + -LEFT_Z) / 2 + proportion;
	float z2 = (LEFT_Z + -LEFT_Z) / 2 - proportion;
	float y1 = DECK_LEVEL - DECK_HEIGHT;
	glm::vec3 steer1 = { x1, y1, z1 };			//10
	glm::vec3 steer2 = { x1, y1, z2 };			//11

	glm::vec3 steer3 = { x1 - abs(steer1[2] - steer2[2]), y1, z1 };		//12
	glm::vec3 steer4 = { x1 - abs(steer1[2] - steer2[2]), y1, z2 };

	glm::vec3 steer5 = { x1, y1 - 0.5*(abs(DECK_LEVEL - y1)), z1 };		//14
	glm::vec3 steer6 = { x1, y1 - 0.5*(abs(DECK_LEVEL - y1)), z2 };

	glm::vec3 steer7 = { x1 - abs(steer1[2] - steer2[2]),  y1 - 0.5*(abs(DECK_LEVEL - y1)), z1 };	//16
	glm::vec3 steer8 = { x1 - abs(steer1[2] - steer2[2]),  y1 - 0.5*(abs(DECK_LEVEL - y1)), z2 };

	glm::vec3 steer9 = { x1,  y1 - (abs(DECK_LEVEL - y1)), z1 };		//18
	glm::vec3 steer10 = { x1,  y1 - (abs(DECK_LEVEL - y1)), z2 };

	glm::vec3 steer11 = { x1 - abs(steer1[2] - steer2[2]),  y1 - (abs(DECK_LEVEL - y1)), z1 };//20
	glm::vec3 steer12 = { x1 - abs(steer1[2] - steer2[2]),  y1 - (abs(DECK_LEVEL - y1)), z2 };

	glm::vec3 steer13 = { x1 - 5 * abs(steer1[2] - steer2[2]),  y1 - 0.5*(abs(DECK_LEVEL - y1)), (LEFT_Z + -LEFT_Z) / 2 };	//22
	glm::vec3 steer14 = { x1 - 5 * abs(steer1[2] - steer2[2]), y1 - (abs(DECK_LEVEL - y1)), (LEFT_Z + -LEFT_Z) / 2 };
	result.push_back(steer1);
	result.push_back(steer2);
	result.push_back(steer3);
	result.push_back(steer4);
	result.push_back(steer5);
	result.push_back(steer6);
	result.push_back(steer7);
	result.push_back(steer8);
	result.push_back(steer9);
	result.push_back(steer10);
	result.push_back(steer11);
	result.push_back(steer12);
	result.push_back(steer13);
	result.push_back(steer14);
	return result;
}
std::vector<unsigned int> draw_deck_i(int nr_points)
{
	std::vector<unsigned int> result = {
		0, 1, 2,
		1, 2, 3,
		1, 3, 4,
		3, 4, 7,
		2, 3, 6,
		3, 6, 7,
		0, 1, 4,
		0, 4, 5,
		4, 5, 6,
		4, 6, 7,
		0, 2, 6,
		0, 6, 5,
		5, 4, 8,
		5, 6, 8,
		6, 8, 9,
		4, 7, 8,
		7, 8, 9,
		6, 7, 9,
		10, 11, 19,
		10, 18, 19,
		10, 12, 18,
		12, 18, 20,
		12, 13, 20,
		13, 21, 20,
		11, 13, 19,
		13, 19, 21,
		16, 22, 17,
		20, 21, 23,
		16, 20, 22,
		20, 22, 23,
		20, 21, 22,
		21, 22, 23
	};
	return result;
}

//```````````````````````````````````rotator
std::vector<glm::vec3> draw_rotator_v(float x, float y, float z, float r1, float r2, float width, int nr_points)
{
	std::vector<glm::vec3> vertices;

	for (float i = 0; i <= nr_points; ++i)
	{
			float u = i / (float)(nr_points * 2),
			nx = x + r1 * cos(u * 2.0f*M_PI),
			ny = y + r1 * sin(u * 2.0f*M_PI);

		vertices.push_back(glm::vec3(nx, ny, z));
	}

	for (float i = 0; i <= nr_points; ++i)
	{
		float u = i / (float)(nr_points * 2),
			nx = x + r2 * cos(u * 2.0f*M_PI),
			ny = y + r2 * sin(u * 2.0f*M_PI);

		vertices.push_back(glm::vec3(nx, ny, z));
	}

	for (float i = 0; i <= nr_points; ++i)
	{
		float u = i / (float)(nr_points * 2),
			nx = x + r1 * cos(u * 2.0f*M_PI),
			ny = y + r1 * sin(u * 2.0f*M_PI);

		vertices.push_back(glm::vec3(nx, ny, z+width));
	}

	for (float i = 0; i <= nr_points; ++i)
	{
		float u = i / (float)(nr_points * 2),
			nx = x + r2 * cos(u * 2.0f*M_PI),
			ny = y + r2 * sin(u * 2.0f*M_PI);

		vertices.push_back(glm::vec3(nx, ny, z+width));
	}
	
	return vertices;
}
std::vector<unsigned int> draw_rotator_i(int nr_points)
{
	std::vector<unsigned int> indices;

	for (int i = 0; i < nr_points; ++i)
	{
		indices.push_back(i);
		indices.push_back(i+nr_points+1);
		indices.push_back(i+nr_points+2);

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + nr_points + 2);
	}

	for (int i = 2 * nr_points + 2; i < 3 * nr_points + 2; ++i)
	{
		indices.push_back(i);
		indices.push_back(i + nr_points + 1);
		indices.push_back(i + nr_points + 2);

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + nr_points + 2);
	}

	for (int i = 0; i < nr_points; ++i)
	{
		indices.push_back(i);
		indices.push_back(i + 2 * nr_points + 2);
		indices.push_back(i + 2 * nr_points + 3);

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2 * nr_points + 3);
	}

	for (int i = nr_points + 1; i < 2 * nr_points + 1; ++i)
	{
		indices.push_back(i);
		indices.push_back(i + 2 * nr_points + 2);
		indices.push_back(i + 2 * nr_points + 3);

		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(i + 2 * nr_points + 3);
	}

	indices.push_back(0);
	indices.push_back(nr_points + 1);
	indices.push_back(3 * nr_points + 3);
	indices.push_back(0);
	indices.push_back(2 * nr_points + 2);
	indices.push_back(3 * nr_points + 3);

	indices.push_back(nr_points);
	indices.push_back(3 * nr_points + 2);
	indices.push_back(2 * nr_points + 1);
	indices.push_back(2 * nr_points + 1);
	indices.push_back(3 * nr_points + 2);
	indices.push_back(4 * nr_points + 3);

	return indices;
}
std::vector<glm::vec3> draw_paddles_v(float x, float y, float z, float r1, float width, float length)
{
	std::vector<glm::vec3> vertices;
	float height = r1 - 0.05f;
	z -= 0.001f;

	vertices.push_back(glm::vec3(x - width/2, y, z));
	vertices.push_back(glm::vec3(x + width/2, y, z));
	vertices.push_back(glm::vec3(x - width / 2, y, z + length));
	vertices.push_back(glm::vec3(x + width / 2, y, z + length));

	vertices.push_back(glm::vec3(x - width / 2, y + height, z));
	vertices.push_back(glm::vec3(x + width / 2, y + height, z));
	vertices.push_back(glm::vec3(x - width / 2, y + height, z + length));
	vertices.push_back(glm::vec3(x + width / 2, y + height, z + length));

	vertices.push_back(glm::vec3(x - width / 2, y - height, z));
	vertices.push_back(glm::vec3(x + width / 2, y - height, z));
	vertices.push_back(glm::vec3(x - width / 2, y - height, z + length));
	vertices.push_back(glm::vec3(x + width / 2, y - height, z + length));


	vertices.push_back(glm::vec3(x, y + width/2, z));
	vertices.push_back(glm::vec3(x, y - width/2, z));
	vertices.push_back(glm::vec3(x, y + width / 2, z + length));
	vertices.push_back(glm::vec3(x, y - width / 2, z + length));

	vertices.push_back(glm::vec3(x + height, y + width / 2, z));
	vertices.push_back(glm::vec3(x + height, y - width / 2, z));
	vertices.push_back(glm::vec3(x + height, y + width / 2, z + length));
	vertices.push_back(glm::vec3(x + height, y - width / 2, z + length));

	vertices.push_back(glm::vec3(x - height, y + width / 2, z));
	vertices.push_back(glm::vec3(x - height, y - width / 2, z));
	vertices.push_back(glm::vec3(x - height, y + width / 2, z + length));
	vertices.push_back(glm::vec3(x - height, y - width / 2, z + length));

	return vertices;
}
std::vector<unsigned int> draw_paddles_i()
{
	std::vector<unsigned int> indices;

	indices.push_back(0); indices.push_back(1); indices.push_back(5);
	indices.push_back(0); indices.push_back(4); indices.push_back(5);

	indices.push_back(0); indices.push_back(2); indices.push_back(4);
	indices.push_back(2); indices.push_back(4); indices.push_back(6);

	indices.push_back(2); indices.push_back(3); indices.push_back(6);
	indices.push_back(3); indices.push_back(6); indices.push_back(7);

	indices.push_back(1); indices.push_back(3); indices.push_back(5);
	indices.push_back(3); indices.push_back(5); indices.push_back(7);

	//
	indices.push_back(0); indices.push_back(1); indices.push_back(8);
	indices.push_back(1); indices.push_back(8); indices.push_back(9);

	indices.push_back(0); indices.push_back(2); indices.push_back(8);
	indices.push_back(2); indices.push_back(8); indices.push_back(10);

	indices.push_back(2); indices.push_back(3); indices.push_back(10);
	indices.push_back(3); indices.push_back(10); indices.push_back(11);

	indices.push_back(1); indices.push_back(3); indices.push_back(9);
	indices.push_back(3); indices.push_back(9); indices.push_back(11);

	//
	indices.push_back(12); indices.push_back(13); indices.push_back(17);
	indices.push_back(12); indices.push_back(16); indices.push_back(17);

	indices.push_back(12); indices.push_back(14); indices.push_back(16);
	indices.push_back(14); indices.push_back(16); indices.push_back(18);

	indices.push_back(14); indices.push_back(15); indices.push_back(18);
	indices.push_back(15); indices.push_back(18); indices.push_back(19);

	indices.push_back(13); indices.push_back(15); indices.push_back(17);
	indices.push_back(15); indices.push_back(17); indices.push_back(19);

	//
	indices.push_back(12); indices.push_back(13); indices.push_back(21);
	indices.push_back(12); indices.push_back(21); indices.push_back(20);

	indices.push_back(12); indices.push_back(14); indices.push_back(20);
	indices.push_back(14); indices.push_back(20); indices.push_back(22);

	indices.push_back(14); indices.push_back(15); indices.push_back(22);
	indices.push_back(15); indices.push_back(22); indices.push_back(23);

	indices.push_back(13); indices.push_back(15); indices.push_back(23);
	indices.push_back(13); indices.push_back(21); indices.push_back(23);

	return indices;
}
std::vector<glm::vec3> draw_circle_v(float x, float y, float z, float r, int nr_points)
{
	std::vector<glm::vec3> vertices;

	vertices.push_back(glm::vec3(x, y, z));
	for (float i = 0; i < nr_points; ++i)
	{
		float u = i / (float)nr_points,
			nx = x + r * cos(u * 2.0f*M_PI),
			ny = y + r * sin(u * 2.0f*M_PI);

		vertices.push_back(glm::vec3(nx, ny, z));
	}
	return vertices;
}
std::vector<unsigned int> draw_circle_i(int nr_points)
{
	std::vector<unsigned int> indices;

	for (int i = 1; i < nr_points; ++i)
	{
		indices.push_back(0);
		indices.push_back(i);
		indices.push_back(i+1);

	}
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(nr_points);

	return indices;
}

