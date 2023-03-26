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
unsigned int createProgram(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader);
unsigned int ModelVAO(Object* model);
void LoadTexture(unsigned int& texture, const char* tFileName);
unsigned int mode = 0;
glm::mat4 getPerspective();
glm::mat4 getView();

Object* whaleModel = new Object("obj/whale.obj");
Object* catModel = new Object("obj/cat.obj");
Object* boxModel = new Object("obj/CardboardBox1.obj");
Object* platModel = new Object("obj/plat.obj");

int windowWidth = 800, windowHeight = 600;

float angle = 0;
float lighting = 1;
float squeezeFactor = 0;
bool squeezing = false;
bool lightChanging = false;

int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW4", NULL, NULL);
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

	// Shaders
	unsigned int vertexShader, fragmentShader, geometryShader, shaderProgram;
	vector<unsigned int> whale_programs, plat_programs, whale_star_programs, 
						 current_whale_programs, current_plat_programs, whale_init_programs, 
					     whale_color_programs, plat_color_programs;
	
	
	vertexShader = createShader("shaders/whale.vert", "vert");
	fragmentShader = createShader("shaders/whale.frag", "frag");
	shaderProgram = createProgram(vertexShader, 0, fragmentShader);
	whale_programs.push_back(shaderProgram);
	whale_star_programs.push_back(shaderProgram);
	whale_init_programs.push_back(shaderProgram);

	vertexShader = createShader("shaders/normal.vert", "vert");
	geometryShader = createShader("shaders/whale.geom", "geom");
	fragmentShader = createShader("shaders/normal.frag", "frag");
	shaderProgram = createProgram(vertexShader, geometryShader, fragmentShader);
	whale_programs.push_back(shaderProgram);

	vertexShader = createShader("shaders/normal.vert", "vert");
	geometryShader = createShader("shaders/whale_star.geom", "geom");
	fragmentShader = createShader("shaders/normal.frag", "frag");
	shaderProgram = createProgram(vertexShader, geometryShader, fragmentShader);
	whale_star_programs.push_back(shaderProgram);

	vertexShader = createShader("shaders/plat.vert", "vert");
	fragmentShader = createShader("shaders/plat.frag", "frag");
	shaderProgram = createProgram(vertexShader, 0, fragmentShader);
	plat_programs.push_back(shaderProgram);

	vertexShader = createShader("shaders/whale_2.vert", "vert");
	fragmentShader = createShader("shaders/whale_2.frag", "frag");
	shaderProgram = createProgram(vertexShader, 0, fragmentShader);
	whale_color_programs.push_back(shaderProgram);

	vertexShader = createShader("shaders/normal.vert", "vert");
	geometryShader = createShader("shaders/whale_color.geom", "geom");
	fragmentShader = createShader("shaders/normal.frag", "frag");
	shaderProgram = createProgram(vertexShader, geometryShader, fragmentShader);
	whale_color_programs.push_back(shaderProgram);

	vertexShader = createShader("shaders/plat_2.vert", "vert");
	fragmentShader = createShader("shaders/plat_2.frag", "frag");
	shaderProgram = createProgram(vertexShader, 0, fragmentShader);
	plat_color_programs.push_back(shaderProgram);

	// Texture
	unsigned int catTexture, boxTexture, whale_star_Texture, plat_star_Texture, whaleTexture, platTexture, current_whale, current_plat;
	//LoadTexture(catTexture, "obj/Cat_diffuse.jpg");
	//LoadTexture(boxTexture, "obj/CardboardBox1_Albedo.tga");
	LoadTexture(whale_star_Texture, "obj/whale2.jpg");
	LoadTexture(plat_star_Texture, "obj/plat3.jpg");
	LoadTexture(whaleTexture, "obj/whale.jpg");
	LoadTexture(platTexture, "obj/plat1.jpg");

	// VAO, VBO
	unsigned int catVAO, boxVAO, whaleVAO, platVAO;
	catVAO = ModelVAO(catModel);
	boxVAO = ModelVAO(boxModel);
	whaleVAO = ModelVAO(whaleModel); 
	platVAO = ModelVAO(platModel);

	// Display loop
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	double dt;
	double lastTime = glfwGetTime();
	double currentTime;
	unsigned int mLoc, vLoc, pLoc, normalMLoc;

	cout << "Press 0: Init, press 1: mode1, press 2: mode2, press 3: party time :D\n";
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// TO DO:
		glm::mat4 perspective = getPerspective();
		glm::mat4 view = getView();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 plat_model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(0, 0.3, 0));
		model = glm::rotate(model, angle, glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		model = glm::scale(model, glm::vec3(0.003, 0.003, 0.003));


		plat_model = glm::rotate(plat_model, angle, glm::vec3(0, 1, 0));
		plat_model = glm::translate(plat_model, glm::vec3(0, -0.8, 0));
		//plat_model = glm::scale(plat_model, glm::vec3(0.3, 0.3, 0.03));

		glm::mat4 normalM = glm::transpose(glm::inverse(model));

		size_t n = whale_programs.size();
		size_t m = plat_programs.size();

		if (mode == 0) {
			current_whale_programs = whale_init_programs;
			current_whale = whaleTexture;
			current_plat = platTexture;
			current_plat_programs = plat_programs;
		}
		else if (mode == 1) {
			current_whale_programs = whale_programs;
			current_whale = whaleTexture;
			current_plat = platTexture;
			current_plat_programs = plat_programs;
		}
		else if (mode == 2) {
			current_whale_programs = whale_star_programs;
			current_whale = whale_star_Texture;
			current_plat = plat_star_Texture;
			current_plat_programs = plat_programs;
		}
		else if (mode == 3) {
			current_whale_programs = whale_color_programs;
			current_whale = whaleTexture;
			current_plat = platTexture;
			current_plat_programs = plat_color_programs;
		}

		for (unsigned int program : current_whale_programs) {
			glUseProgram(program);
			mLoc = glGetUniformLocation(program, "M");
			vLoc = glGetUniformLocation(program, "V");
			pLoc = glGetUniformLocation(program, "P");
			normalMLoc = glGetUniformLocation(program, "normalM");
			glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(model));
			glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(perspective));
			glUniformMatrix4fv(normalMLoc, 1, GL_FALSE, glm::value_ptr(normalM));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, current_whale);
			glBindVertexArray(whaleVAO);
			glDrawArrays(GL_TRIANGLES, 0, whaleModel->positions.size());
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
		}

		for (unsigned int program : current_plat_programs) {
			glUseProgram(program);
			mLoc = glGetUniformLocation(program, "M");
			vLoc = glGetUniformLocation(program, "V");
			pLoc = glGetUniformLocation(program, "P");
			normalMLoc = glGetUniformLocation(program, "normalM");
			glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(plat_model));
			glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(perspective));
			glUniformMatrix4fv(normalMLoc, 1, GL_FALSE, glm::value_ptr(normalM));
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, current_plat);
			glBindVertexArray(platVAO);
			glDrawArrays(GL_TRIANGLES, 0, platModel->positions.size());
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
		}

		// Status update
		currentTime = glfwGetTime();
		dt = currentTime - lastTime;
		lastTime = currentTime;
		angle += glm::radians(45.0f) * dt;
		if (angle > glm::radians(360.0f))	angle -= glm::radians(360.0f);
		lighting = 0.75f + 0.25 * cos(angle);

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
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		mode = 0;
		cout << "Init\n";
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		mode = 1;
		cout << "Light mode 1\n";
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		mode = 2;
		cout << "Light mode 2\n";
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		mode = 3;
		cout << "Light mode 3\n";
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
	else if (strcmp(type, "geom") == 0)
		shader = glCreateShader(GL_GEOMETRY_SHADER);
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

unsigned int createProgram(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, geometryShader);
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

		cout << "LINKING ERROR: ";
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

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
		
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

glm::mat4 getPerspective()
{
	return glm::perspective(
		glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 
		1.0f, 100.0f);
}

glm::mat4 getView()
{
	return glm::lookAt(
		//glm::vec3(0, 4, 4),
		glm::vec3(0, 0, 4),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
}