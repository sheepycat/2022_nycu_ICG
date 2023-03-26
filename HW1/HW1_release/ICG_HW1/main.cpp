#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int createShader(const char* filename, const char* type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int ModelVAO(Object* model);
void DrawModel(const char* target, glm::mat4 M, glm::mat4 V, glm::mat4 P);

Object* catModel = new Object("obj/cat.obj");
Object* baseModel = new Object("obj/platform.obj");
Object* teapotModel = new Object("obj/teapot.obj");
Object* ballModel = new Object("obj/tennis_ball.obj");
Object* diskModel = new Object("obj/disk.obj");
unsigned int catVAO, baseVAO, ballVAO, teapotVAO, diskVAO;
unsigned int shaderProgram;
int windowWidth = 800, windowHeight = 600;
float k = 1;
int state = 0;;

int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW1", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// VAO, VBO
	catVAO = ModelVAO(catModel);
	baseVAO = ModelVAO(baseModel);
	ballVAO = ModelVAO(ballModel);
	teapotVAO = ModelVAO(teapotModel);
	diskVAO = ModelVAO(diskModel);

	// Shaders
	unsigned int vertexShader, fragmentShader;
	vertexShader = createShader("vertexShader.vert", "vert");
	fragmentShader = createShader("fragmentShader.frag", "frag");
	shaderProgram = createProgram(vertexShader, fragmentShader);
	glUseProgram(shaderProgram);

	// TO DO:
	// Enable depth test, face culling
	///////prevent occluded faces  being rendered
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);//: test passes if fragment depth <= depth stored in buffer 
	///
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	// Display loop
	// TO DO:
	// Draw the rotating music box
	///clear the color buffer and depth buffer
	
	//////
	
	glm::mat4 view = glm::mat4(1.0f); //initialize matrix: identity matrix
	glm::mat4 prespective = glm::mat4(1.0f);
	view = glm::lookAt(glm::vec3(0, 12, 12),
		glm::vec3(0, -1, 0),
		glm::vec3(0, 1, 0));
	prespective = glm::perspective(glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	glViewport(0, 0, windowWidth, windowHeight);

	glm::mat4 base_model = glm::mat4(1.0f);
	glm::mat4 base_tmp = glm::mat4(1.0f);
	glm::mat4 base_r = glm::mat4(1.0f);
	base_r = glm::rotate(base_r, glm::radians(-0.2f), glm::vec3(0.0f, 1.0f, 0.0f));//angle axis

	glm::mat4 disk_model = glm::mat4(1.0f);
	glm::mat4 disk_tmp = glm::mat4(1.0f);
	glm::mat4 disk_r = glm::mat4(1.0f);
	disk_model = glm::translate(disk_model, glm::vec3(0, 1.3, 0));
	disk_r = glm::rotate(disk_r, glm::radians(-0.7f * k), glm::vec3(0.0f, 1.0f, 0.0f));//angle axis

	glm::mat4 teapot_disk_model = glm::mat4(1.0f);
	glm::mat4 teapot_disk_tmp = glm::mat4(1.0f);
	glm::mat4 teapot_disk_r = glm::mat4(1.0f);
	teapot_disk_model = glm::translate(teapot_disk_model, glm::vec3(-1, 1.5, 0));
	teapot_disk_r = glm::rotate(teapot_disk_r, glm::radians(-0.2f), glm::vec3(0.0f, 1.0f, 0.0f));//angle axis

	glm::mat4 teapot_model = glm::mat4(1.0f);
	glm::mat4 teapot_tmp = glm::mat4(1.0f);
	glm::mat4 teapot_r = glm::mat4(1.0f);
	teapot_model = glm::translate(teapot_model, glm::vec3(-1, 1.5, 0));
	teapot_r = glm::rotate(teapot_r, glm::radians(-0.2f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 cat_model = glm::mat4(1.0f);
	glm::mat4 cat_tmp = glm::mat4(1.0f);
	cat_model = glm::translate(cat_model, glm::vec3(2.5, 1.3, 0));
	

	glm::mat4 ball_model = glm::mat4(1.0f); 
	glm::mat4 ball_tmp = glm::mat4(1.0f);
	ball_model = glm::translate(ball_model, glm::vec3(2.5, 1.8, 2));

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		/// TO DO:
		// Create model, view, and perspective matrix
		if (state == 1) {
			disk_r = glm::rotate(glm::mat4(1.0f), glm::radians(-0.7f * k), glm::vec3(0.0f, 1.0f, 0.0f));//angle axis
			state = 0;
		}
		
		
		base_model = base_r * base_model;
		base_tmp = base_model;
		base_model = glm::scale(base_model, glm::vec3(1.25, 1, 1.25));
		DrawModel("base", base_model, view, prespective);
		base_model = base_tmp;
		
		disk_model = disk_r * disk_model;
		disk_tmp = disk_model;
		disk_model = glm::scale(disk_model, glm::vec3(4, 1, 4));
		DrawModel("disk", disk_model, view, prespective);
		disk_model = disk_tmp;
		
		teapot_disk_model = glm::rotate(teapot_disk_model, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));//angle axis
		teapot_disk_model = teapot_disk_r * teapot_disk_model;
		teapot_disk_tmp = teapot_disk_model;
		teapot_disk_model = glm::scale(teapot_disk_model, glm::vec3(1, 1, 1));
		DrawModel("disk", teapot_disk_model, view, prespective);
		teapot_disk_model = teapot_disk_tmp;

		
		teapot_model = glm::rotate(teapot_model, glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f));//angle axis
		teapot_model = teapot_r * teapot_model;
		teapot_tmp = teapot_model;
		teapot_model = glm::scale(teapot_model, glm::vec3(0.3, 0.3, 0.3));
		DrawModel("teapot", teapot_model, view, prespective);
		teapot_model = teapot_tmp;

		
		cat_model = disk_r * cat_model;
		cat_tmp = cat_model;
		cat_model = glm::scale(cat_model, glm::vec3(1, 1, 1));
		DrawModel("cat", cat_model, view, prespective);
		cat_model = cat_tmp;

		
		ball_model = glm::rotate(ball_model, glm::radians(1.2f), glm::vec3(1.0f, 0.0f, 0.0f));//angle axis
		ball_model = disk_r * ball_model;
		ball_tmp = ball_model;
		ball_model = glm::scale(ball_model, glm::vec3(1.2, 1.2, 1.2));
		DrawModel("ball", ball_model, view, prespective);
		ball_model = ball_tmp;

		
		//////
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// TO DO:
// Add key callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
		k = k * 2;
		state = 1;
	}
		
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
		k = k / 2;
		state = 1;
	}
		
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

unsigned int createShader(const char* filename, const char* type)
{
	unsigned int shader;
	if (strcmp(type, "vert") == 0)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (strcmp(type, "frag") == 0)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}

	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //same as rewind(fp);

	char* source = (char*)malloc(sizeof(char) * (fsize + 1));
	fread(source, fsize, 1, fp);
	fclose(fp);

	source[fsize] = 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << type << "::COMPLIATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		char* infoLog = (char*)malloc(sizeof(char) * (maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		puts(infoLog);
		free(infoLog);

		return -1;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

unsigned int ModelVAO(Object* model)
{
	unsigned int VAO, VBO[3];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->positions.size()), &(model->positions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->normals.size()), &(model->normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->texcoords.size()), &(model->texcoords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

void DrawModel(const char* target, glm::mat4 M, glm::mat4 V, glm::mat4 P)
{
	unsigned int mLoc, vLoc, pLoc;
	mLoc = glGetUniformLocation(shaderProgram, "M");
	vLoc = glGetUniformLocation(shaderProgram, "V");
	pLoc = glGetUniformLocation(shaderProgram, "P");
	glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(M));
	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(V));
	glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(P));
	if (strcmp(target, "base") == 0)
	{
		glBindVertexArray(baseVAO);
		glDrawArrays(GL_TRIANGLES, 0, baseModel->positions.size());
	}
	else if (strcmp(target, "cat") == 0)
	{
		glBindVertexArray(catVAO);
		glDrawArrays(GL_TRIANGLES, 0, catModel->positions.size());
	}
	else if (strcmp(target, "ball") == 0)
	{
		glBindVertexArray(ballVAO);
		glDrawArrays(GL_TRIANGLES, 0, ballModel->positions.size());
	}
	else  if (strcmp(target, "teapot") == 0)
	{
		glBindVertexArray(teapotVAO);
		glDrawArrays(GL_TRIANGLES, 0, teapotModel->positions.size());
	}
	else if (strcmp(target, "disk") == 0)
	{
		glBindVertexArray(diskVAO);
		glDrawArrays(GL_TRIANGLES, 0, diskModel->positions.size());
	}
	glBindVertexArray(0);
}
