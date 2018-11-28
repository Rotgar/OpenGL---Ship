#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shader/Shader.hpp>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

std::vector<glm::vec3> draw_cylinder_v(float x, float y, float z, float r1, float r2, float r3, int o, int nr_points);
std::vector<unsigned int> draw_cylinder_i(int nr_points, int o);

std::vector<glm::vec3> draw_cuboid_v(float x, float y, float z, float length, float width, float height);
std::vector<unsigned int> draw_cuboid_i();

// settings
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 1000;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
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

float shipMovement = 0.0f, acceleration = 0.005f, rotation = 0.0f;
bool stand = true, forward = false , backward = false, left = false, right = false;



glm::vec4 rotateAround(const glm::vec4& aPointToRotate, const glm::vec4&  aRotationCenter, const glm::mat4x4& aRotationMatrix)
{
	glm::mat4x4 translate = glm::translate(glm::mat4(), glm::vec3(aRotationCenter.x, aRotationCenter.y, aRotationCenter.z)); 
	glm::mat4x4 invTranslate = glm::inverse(translate);

	// The idea:
	// 1) Translate the object to the center
	// 2) Make the rotation
	// 3) Translate the object back to its original location

	glm::mat4x4 transform = translate * aRotationMatrix * invTranslate;

	return transform * aPointToRotate;
}

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
		   cylinder("Shaders\\shader_c.vs", "Shaders\\shader_c.fs"),
		simple_shader("Shaders\\shader_simple.vs", "Shaders\\shader_simple.fs");

	float vertices[] = {
		// positions         // colors
		-0.6f, -0.7f, 0.5f,  1.0f, 1.0f, 0.0f,  // bottom left front     0
		 0.7f, -0.7f, 0.5f,  0.0f, 1.0f, 0.2f,  // bottom right front    1
		-0.8f, -0.4f, 0.5f,  0.5f, 0.0f, 1.0f,  // top left front		 2
		 0.8f, -0.4f, 0.5f,  1.0f, 0.2f, 0.5f,  // top right front		 3
		
		-0.6f, -0.7f, -0.5f,  1.0f, 1.0f, 0.0f,  // bottom left behind	 4
		 0.7f, -0.7f, -0.5f,  1.0f, 0.5f, 1.0f,  // bottom right behind  5
		-0.8f, -0.4f, -0.5f,  0.2f, 0.5f, 1.0f,  // top left behind		 6
		 0.8f, -0.4f, -0.5f,  1.0f, 0.5f, 0.5f   // top right behind	 7
	};
	
	unsigned int indices[] = {
		
		//glowny 12
		0, 1, 2,
		1, 2, 3,
		1, 5, 7,
		1, 3, 7,
		2, 3, 6,
		3, 6, 7,
		0, 1, 4,
		1, 4, 5,
		4, 5, 6,
		5, 6, 7,
		0, 4, 6,
		0, 6, 2
	};

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

	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAOChimney);
	glGenVertexArrays(1, &VAOCrane);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBOChimney);
	glGenBuffers(1, &VBOCrane);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &EBOChimney);
	glGenBuffers(1, &EBOCrane);

	//main part
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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



	// sea---------------------
	unsigned int VAO_sea, VBO_sea;
	glGenVertexArrays(1, &VAO_sea);
	glGenBuffers(1, &VBO_sea);
	glBindVertexArray(VAO_sea);

	float vertices_sea[] = {

		-1000.0f, -1.1f, -1000.0f,
		 1000.0f, -1.1f, -1000.0f,
		 1000.0f, -1.1f,  1000.0f,

		 1000.0f, -1.1f,  1000.0f,
		-1000.0f, -1.1f,  1000.0f,
		-1000.0f, -1.1f, -1000.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO_sea);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_sea), vertices_sea, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glm::mat4 VAOmodel = glm::mat4(1.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	float angle = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		if (!stand)
		{
			if (forward)
				shipMovement += acceleration;
			else if (backward)
				shipMovement -= acceleration;
		}

		glClearColor(0.45f, 0.8f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		ourShader.use();
		unsigned int colorLoc = glGetUniformLocation(ourShader.ID, "newColor");

		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		ourShader.setMat4("projection", projection);

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		ourShader.setMat4("view", view);
		

		glBindVertexArray(VAO_sea);
		glm::mat4 model = glm::mat4(1.0f);
		ourShader.setMat4("model", model);
		glUniform4f(colorLoc, 0.1, 0.3, 0.9, 1.0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		glBindVertexArray(VAO);
		
		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(shipMovement, 0, 0));
		model = glm::scale(model, glm::vec3(2.0f, 1.5f, 1.5f));
		ourShader.setMat4("model", model);

		glUniform4f(colorLoc, 0.5, 0.3, 0.1, 1.0);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(VAOCrane);
		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(shipMovement, 0.1, 0));
	
		model = glm::scale(model, glm::vec3(1.5f, 2.0f, 0.8f));
		ourShader.setMat4("model", model);

		glUniform4f(colorLoc, 1.0, 0.0, 0.0, 1.0);
		glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_INT, 0);

		
		glBindVertexArray(VAO_c1);
		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(0.7 + shipMovement, -0.6, 0.3));
		model = glm::rotate(model, glm::radians(22.0f), glm::vec3(0, 1, 0));
		
		ourShader.setMat4("model", model);
		glUniform4f(colorLoc, 0.0, 1.0, 0.0, 1.0);
		glDrawElements(GL_TRIANGLES, indices_c1.size(), GL_UNSIGNED_INT, 0);

		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(0.7 + shipMovement, -0.6, -0.3));
		model = glm::rotate(model, glm::radians(47.0f), glm::vec3(0, 1, 0));
		ourShader.setMat4("model", model);
		glUniform4f(colorLoc, 0.4, 0.4, 0.5, 1.0);
		glDrawElements(GL_TRIANGLES, indices_c1.size(), GL_UNSIGNED_INT, 0);


		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-1.0 + shipMovement, -0.6, 0));
		model = glm::scale(model, glm::vec3(2.5f, 1.6f, 1.8f));
		ourShader.setMat4("model", model);
		glUniform4f(colorLoc, 0.0, 1.0, 0.0, 1.0);
		glDrawElements(GL_TRIANGLES, indices_c1.size(), GL_UNSIGNED_INT, 0);





		cylinder.use();
		unsigned int cylinder_colorLoc = glGetUniformLocation(cylinder.ID, "newColor");

		glBindVertexArray(VAO_c);
		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(rotation), glm::vec3(0, 1, 0));
		model = glm::translate(model, glm::vec3(-0.4 + shipMovement, -0.6, -0.3));
	
		glUniform4f(cylinder_colorLoc, 1.0, 0.0, 0.0, 1.0);
		//model = glm::translate(model, glm::vec3(0, 0, 0));
		cylinder.setMat4("projection", projection);
		cylinder.setMat4("view", view);
		cylinder.setMat4("model", model);

		glDrawElements(GL_TRIANGLES, indices_c.size(), GL_UNSIGNED_INT, 0);
	

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
		rotation += 5.0f;
		left = true;
		right = false;
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		rotation -= 5.0f;
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















