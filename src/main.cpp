#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "Geometry.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "Primitives.h"

// ============================================================
// Constants
// ============================================================

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

static float rad(float degrees) {
	return degrees * (PI / 180.0f);
}

// ============================================================
// Global State
// ============================================================

Camera camera;
bool sceneRotating = false;
float sceneAngle   = 0.0f;

// ============================================================
// GLFW Callbacks
// ============================================================

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.processMouseMovement(xpos, ypos);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
		sceneRotating = !sceneRotating;
}

// ============================================================
// Per-Frame Input
// ============================================================

void processInput(GLFWwindow* window, float deltaTime) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// WASD Movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(Camera::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(Camera::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(Camera::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(Camera::RIGHT, deltaTime);

	if (sceneRotating)
		sceneAngle += 1.0f * deltaTime;
}

// ============================================================
// Shader Source (Blinn-Phong)
// ============================================================

static const char* litVertSrc =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec4 aColor;\n"
	"layout (location = 2) in vec2 aTexCoord;\n"
	"layout (location = 3) in vec3 aNormal;\n"
	"\n"
	"out vec4 ourColor;\n"
	"out vec2 TexCoord;\n"
	"out vec3 FragPos;\n"
	"out vec3 Normal;\n"
	"\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"\n"
	"void main() {\n"
	"    FragPos = vec3(model * vec4(aPos, 1.0));\n"
	"    Normal = mat3(transpose(inverse(model))) * aNormal;\n"
	"    gl_Position = projection * view * vec4(FragPos, 1.0);\n"
	"    ourColor = aColor;\n"
	"    TexCoord = aTexCoord;\n"
	"}\n";

static const char* litFragSrc =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"\n"
	"in vec4 ourColor;\n"
	"in vec2 TexCoord;\n"
	"in vec3 FragPos;\n"
	"in vec3 Normal;\n"
	"\n"
	"uniform sampler2D texture1;\n"
	"uniform vec3 lightDir;\n"
	"uniform vec3 lightColor;\n"
	"uniform vec3 ambientColor;\n"
	"uniform vec3 viewPos;\n"
	"uniform float specularStrength;\n"
	"\n"
	"void main() {\n"
	"    vec4 texColor = texture(texture1, TexCoord) * ourColor;\n"
	"    vec3 objectColor = texColor.rgb;\n"
	"    vec3 norm = normalize(Normal);\n"
	"\n"
	"    // Ambient\n"
	"    vec3 ambient = ambientColor * objectColor;\n"
	"\n"
	"    // Diffuse\n"
	"    vec3 lightDirNorm = normalize(-lightDir);\n"
	"    float diff = max(dot(norm, lightDirNorm), 0.0);\n"
	"    vec3 diffuse = diff * lightColor * objectColor;\n"
	"\n"
	"    // Specular (Blinn-Phong)\n"
	"    vec3 viewDir = normalize(viewPos - FragPos);\n"
	"    vec3 halfwayDir = normalize(lightDirNorm + viewDir);\n"
	"    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);\n"
	"    vec3 specular = spec * lightColor * specularStrength;\n"
	"\n"
	"    vec3 result = ambient + diffuse + specular;\n"
	"    FragColor = vec4(result, texColor.a);\n"
	"}\n";

// ============================================================
// Main
// ============================================================

int main() {
	// Initialize GLFW
	if (!glfwInit()) return -1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
		"3D Outdoor Scene — Blinn-Phong Lighting", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Set callbacks
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Load OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

	glEnable(GL_DEPTH_TEST);

	// -----------------------------------------------------------
	// Compile shader
	// -----------------------------------------------------------
	Shader litShader(litVertSrc, litFragSrc);
	litShader.use();
	litShader.setInt("texture1", 0);

	// -----------------------------------------------------------
	// Generate procedural textures
	// -----------------------------------------------------------
	unsigned int brickTex  = generateBrickTexture();
	unsigned int roofTex   = generateRoofTexture();
	unsigned int grassTex  = generateGrassTexture();
	unsigned int roadTex   = generateRoadTexture();
	unsigned int barkTex   = generateBarkTexture();
	unsigned int leavesTex = generateLeavesTexture();
	unsigned int doorTex   = generateDoorTexture();
	unsigned int whiteTex  = generateWhiteTexture();

	// -----------------------------------------------------------
	// Create scene geometry
	// -----------------------------------------------------------
	TexturedCube  houseBody(2.0f, 1.5f, 1.5f, Color(1, 1, 1, 1), 3.0f, 2.0f);
	TexturedPyramid roof(2.2f, 1.7f, 1.0f);
	TexturedCube  door(0.5f, 0.9f, 0.05f, Color(1, 1, 1, 1), 1.0f, 1.0f);
	TexturedPlane grass(14.0f, 14.0f, Color(1, 1, 1, 1), 8.0f, 8.0f);
	TexturedPlane road(14.0f, 2.5f, Color(1, 1, 1, 1), 7.0f, 1.0f);
	TexturedCylinder trunk(0.12f, 1.2f, 16);
	TexturedCone  leaves(0.7f, 1.5f, 16);
	TexturedSphere mirrorBall(0.6f, 36, 18, Color(0.9f, 0.9f, 1.0f, 1.0f));

	houseBody.initGL();
	roof.initGL();
	door.initGL();
	grass.initGL();
	road.initGL();
	trunk.initGL();
	leaves.initGL();
	mirrorBall.initGL();

	// -----------------------------------------------------------
	// Directional light setup (sun coming from upper-right-front)
	// -----------------------------------------------------------
	float sunDirX = -0.4f, sunDirY = -0.8f, sunDirZ = -0.4f;

	// -----------------------------------------------------------
	// Render loop
	// -----------------------------------------------------------
	float lastFrame = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = (float)glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window, deltaTime);

		// Sky blue background
		glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// View and projection
		int w, h;
		glfwGetFramebufferSize(window, &w, &h);
		float aspect = (float)w / (float)(h > 0 ? h : 1);
		Mat4 projection = Mat4::perspective(rad(45.0f), aspect, 0.1f, 100.0f);
		Mat4 view = camera.getViewMatrix();

		// Scene rotation
		Mat4 sceneRot = Mat4::rotateY(sceneAngle);

		// Camera position for specular
		float camX = camera.position.x;
		float camY = camera.position.y;
		float camZ = camera.position.z;

		// Bind shader and set per-frame uniforms
		litShader.use();
		litShader.setMat4("view", view.m);
		litShader.setMat4("projection", projection.m);
		litShader.setVec3("lightDir", sunDirX, sunDirY, sunDirZ);
		litShader.setVec3("lightColor", 1.0f, 0.95f, 0.85f);
		litShader.setVec3("ambientColor", 0.25f, 0.25f, 0.3f);
		litShader.setVec3("viewPos", camX, camY, camZ);
		litShader.setFloat("specularStrength", 0.3f); // default specularity
		glActiveTexture(GL_TEXTURE0);

		// --- Grass field ---
		glBindTexture(GL_TEXTURE_2D, grassTex);
		Mat4 grassModel = sceneRot;
		litShader.setMat4("model", grassModel.m);
		grass.draw();

		// --- Road ---
		glBindTexture(GL_TEXTURE_2D, roadTex);
		Mat4 roadModel = sceneRot * Mat4::translate(0.0f, 0.005f, 2.5f);
		litShader.setMat4("model", roadModel.m);
		road.draw();

		// --- House body ---
		glBindTexture(GL_TEXTURE_2D, brickTex);
		Mat4 houseModel = sceneRot * Mat4::translate(0.0f, 0.75f, 0.0f);
		litShader.setMat4("model", houseModel.m);
		houseBody.draw();

		// --- Roof ---
		glBindTexture(GL_TEXTURE_2D, roofTex);
		Mat4 roofModel = sceneRot * Mat4::translate(0.0f, 1.5f, 0.0f);
		litShader.setMat4("model", roofModel.m);
		roof.draw();

		// --- Door ---
		glBindTexture(GL_TEXTURE_2D, doorTex);
		Mat4 doorModel = sceneRot * Mat4::translate(0.3f, 0.45f, 0.76f);
		litShader.setMat4("model", doorModel.m);
		door.draw();

		// --- Mirror Ball (in front of house) ---
		glBindTexture(GL_TEXTURE_2D, whiteTex);
		Mat4 ballModel = sceneRot * Mat4::translate(0.0f, 0.6f, 1.5f); // in front of house
		litShader.setMat4("model", ballModel.m);
		litShader.setFloat("specularStrength", 1.2f); // very shiny
		mirrorBall.draw();
		litShader.setFloat("specularStrength", 0.3f); // restore default

		// --- Tree 1 (left side) ---
		glBindTexture(GL_TEXTURE_2D, barkTex);
		Mat4 trunk1Model = sceneRot * Mat4::translate(-3.0f, 0.0f, -1.0f);
		litShader.setMat4("model", trunk1Model.m);
		trunk.draw();

		glBindTexture(GL_TEXTURE_2D, leavesTex);
		Mat4 leaves1Model = sceneRot * Mat4::translate(-3.0f, 0.9f, -1.0f);
		litShader.setMat4("model", leaves1Model.m);
		leaves.draw();

		// --- Tree 2 (right side) ---
		glBindTexture(GL_TEXTURE_2D, barkTex);
		Mat4 trunk2Model = sceneRot * Mat4::translate(3.0f, 0.0f, -1.0f);
		litShader.setMat4("model", trunk2Model.m);
		trunk.draw();

		glBindTexture(GL_TEXTURE_2D, leavesTex);
		Mat4 leaves2Model = sceneRot * Mat4::translate(3.0f, 0.9f, -1.0f);
		litShader.setMat4("model", leaves2Model.m);
		leaves.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// -----------------------------------------------------------
	// Cleanup
	// -----------------------------------------------------------
	houseBody.cleanup();
	roof.cleanup();
	door.cleanup();
	grass.cleanup();
	road.cleanup();
	trunk.cleanup();
	leaves.cleanup();
	mirrorBall.cleanup();

	glDeleteTextures(1, &brickTex);
	glDeleteTextures(1, &roofTex);
	glDeleteTextures(1, &grassTex);
	glDeleteTextures(1, &roadTex);
	glDeleteTextures(1, &barkTex);
	glDeleteTextures(1, &leavesTex);
	glDeleteTextures(1, &doorTex);
	glDeleteTextures(1, &whiteTex);

	litShader.cleanup();
	glfwTerminate();
	return 0;
}