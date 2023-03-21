#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "camera.h"

#include <thread>
#include <string.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// settings
float ScreenRatio = (float)16 / 9;
const unsigned int SCR_HEIGHT = 675;
const unsigned int SCR_WIDTH = SCR_HEIGHT*ScreenRatio;

// Shaders
std::string vertexShader = "shaders/Mandelbrot.vs";
std::string fragmentShader = "shaders/Mandelbrot.fs";

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//Fractal
glm::dvec2 pos = { -0.460122202, 0.570286 };
double zoom = 2.0;

int main(int argc, char **argv) {

	for (int i = 1; i < argc; i+=2) {
		if (strcmp(argv[i], "-vs") == 0) {
			vertexShader = argv[i + 1];
		}
		else if (strcmp(argv[i], "-fs") == 0) {
			fragmentShader = argv[i + 1];
		}
		else if (strcmp(argv[i], "-p") == 0) {
			char* next_token;
			pos = { std::stod(strtok_s(argv[i + 1], ",", &next_token)), std::stod(strtok_s(NULL, ",", &next_token)) };
		}
		else
			std::cout << "INVALID ARGUMENTS" << std::endl;
	}

	// Instantiate GLFW window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window object
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Register framebuffer_size_callback for resizing
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Register mouse_callback and scroll_callback for mouse mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initializing GLAD before any call of OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to Initialize GLAD" << std::endl;
		return -1;
	}

	// Enable Depth testing
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader program
	Shader shader(vertexShader.c_str(), fragmentShader.c_str());

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Allow drawing in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Render Loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Clear the buffer
		//glClearColor(0.2f, 0.5f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the Program shader
		shader.use();

		shader.setFloat("iTime", (float)glfwGetTime());
		shader.setDVec2("iResolution", SCR_WIDTH, SCR_HEIGHT);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			pos.y += 0.01 * zoom;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			pos.y -= 0.01 * zoom;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			pos.x -= 0.01 * zoom;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			pos.x += 0.01 * zoom;
		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
			zoom *= 1.01;
		if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
			zoom *= 0.99;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		shader.setDVec2("pos", pos.x, pos.y);
		shader.setFloat("zoom", zoom);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate ressources
	shader.programDelete();

	// clean/delete all GLFW's resources allocated
	glfwTerminate();

	return 0;
}

// Function that can resize the window at any time
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}