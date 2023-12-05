
#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>
#include <string>


#include <anm/procGen.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

float SAND_HEIGHT = -10.0f;
float WATER_HEIGHT = 10.0f;
float SEAWEED_HEIGHT = 10.0f; //actual height of the seaweed, not the height it's at
float PLANE_WIDTH = 100.0f;


float prevTime;
ew::Vec3 bgColor = (ew::Vec3(0.0f, 188.0f, 255.0f)/255.0f);

ew::Camera camera;
ew::CameraController cameraController;

bool bp = true;
bool isBrick = false;

const int MAX_LIGHTS = 3;
int numLights = 1;

struct Light
{
	ew::Vec3 position;//World Space position
	ew::Vec3 color;//RGB
};

struct Material
{
	float ambientK; // ambient coefficient (0-1)
	float diffuseK; //Diffuse Coefficient (0-1)
	float specularK; //Specular coefficient (0-1)
	float shininess;//shininess
};

////////////////\\\\\\\\\\\\\\\\
//////FUNCTIONS START HERE\\\\\\
\\\\\\\\\\\\\\\\////////////////

//Clamps the camera within the world border
void clampCameraPos(ew::Camera& camera)
{
	if (camera.position.y <= SAND_HEIGHT+0.1f)
	{
		camera.position.y = SAND_HEIGHT+0.1f;
	}
	if (camera.position.y >= WATER_HEIGHT+0.4f)
	{
		camera.position.y = WATER_HEIGHT+0.4f;
	}
	if (camera.position.x <= -PLANE_WIDTH/2.0f)
	{
		camera.position.x =  (-PLANE_WIDTH / 2.0f);
	}
	if (camera.position.x >= (PLANE_WIDTH / 2.0f))
	{
		camera.position.x = (PLANE_WIDTH / 2.0f);
	}
	if (camera.position.z <= (-PLANE_WIDTH / 2.0f))
	{
		camera.position.z = (-PLANE_WIDTH / 2.0f);
	}
	if (camera.position.z >= (PLANE_WIDTH / 2.0f))
	{
		camera.position.z = (PLANE_WIDTH / 2.0f);
	}
}


//MAIN

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Global settings
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/defaultLit.vert", "assets/defaultLit.frag");
	ew::Shader lightShader("assets/unlit.vert", "assets/unlit.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg", GL_REPEAT, GL_LINEAR);
	unsigned int seaweedTexture = ew::loadTexture("assets/seaweed.jpg", GL_REPEAT, GL_LINEAR);
	unsigned int waterTexture = ew::loadTexture("assets/water_texture.jpg", GL_REPEAT, GL_LINEAR);
	unsigned int sandTexture = ew::loadTexture("assets/sand_texture.jpg", GL_REPEAT, GL_LINEAR);


	//Create meshes and transforms
	ew::Mesh sandMesh(anm::createPlane(PLANE_WIDTH, PLANE_WIDTH, 10, true));
	ew::Mesh waterMesh(ew::createPlane(PLANE_WIDTH, PLANE_WIDTH, 10));
	ew::Mesh seaweedMesh(ew::createPlane(5.0f, SEAWEED_HEIGHT, 5));

	ew::Transform sandTransform;
	ew::Transform waterTransform;
	ew::Transform seaweedTransform;

	sandTransform.position = ew::Vec3(-(PLANE_WIDTH/2.0f), SAND_HEIGHT, (PLANE_WIDTH / 2.0f));
	waterTransform.position = ew::Vec3(0, WATER_HEIGHT, 0);
	seaweedTransform.position = ew::Vec3(0, SAND_HEIGHT + SEAWEED_HEIGHT / 2.0f, 0);
	seaweedTransform.rotation = ew::Vec3(90, 90, 90);


	Material material;
	material.diffuseK = 0.0f;
	material.ambientK = 0.0f;
	material.specularK = 0.0f;
	material.shininess = 2.0f;


	Light light[MAX_LIGHTS];
	ew::Mesh lightMesh(ew::createSphere(2.5f, 64));

	light[0].position = ew::Vec3(0.0f, 2.0f, 0.0f);
	light[0].color = ew::Vec3(1.0f, 0.0f, 0.0f);

	light[1].position = ew::Vec3(1.0f, 2.0f, 0.0f);
	light[1].color = ew::Vec3(0.0f, 1.0f, 0.0f);

	light[2].position = ew::Vec3(-1.0f, 2.0f, 0.0f);
	light[2].color = ew::Vec3(0.0f, 0.0f, 1.0f);

	ew::Transform lightTransform[MAX_LIGHTS];

	for (int i = 0; i < MAX_LIGHTS; i++) {
		lightTransform[i].scale = 0.1;
		lightTransform[i].position = light[i].position;
	}

	resetCamera(camera, cameraController);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glEnable(GL_CULL_FACE);

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		//Update camera
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
		cameraController.Move(window, &camera, deltaTime);
		clampCameraPos(camera);
		shader.setVec3("cameraPos", camera.position);
		shader.setBool("BP", bp);
		shader.setInt("numLights", numLights);

		//BG
		

		shader.setVec3("_Light[0].color", light[0].color);
		shader.setVec3("_Light[0].position", light[0].position);
		lightTransform[0].position = light[0].position;

		shader.setVec3("_Light[1].color", light[1].color);
		shader.setVec3("_Light[1].position", light[1].position);
		lightTransform[1].position = light[1].position;

		shader.setVec3("_Light[2].color", light[2].color);
		shader.setVec3("_Light[2].position", light[2].position);
		lightTransform[2].position = light[2].position;


		shader.setVec3("_Material.diffuseK", material.diffuseK);
		shader.setVec3("_Material.specularK", material.specularK);
		shader.setFloat("_Material.shininess", material.shininess);
		/*
		ew::Vec3 allColor;
		for (int i = 0; i < numLights; i++) {
			allColor += light[i].color;
		}
		*/
		shader.setVec3("ambientColor", bgColor);

		

		//RENDER
		glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightShader.use();
		for (int i = 0; i < numLights; i++) {
			lightShader.setMat4("_Model", lightTransform[i].getModelMatrix());
			lightShader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());
			lightShader.setVec3("_Color", light[i].color);
			lightMesh.draw();
		}

		shader.use();
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		shader.setInt("_Texture", 0);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());

		//Draw shapes
		if (!isBrick)
		{
			glBindTexture(GL_TEXTURE_2D, sandTexture);
		}
		shader.setInt("_Texture", 0);
		shader.setMat4("_Model", sandTransform.getModelMatrix());
		sandMesh.draw();

		glDisable(GL_CULL_FACE);
		if (!isBrick)
		{
			glBindTexture(GL_TEXTURE_2D, waterTexture);
		}
		shader.setInt("_Texture", 0);
		shader.setMat4("_Model", waterTransform.getModelMatrix());
		waterMesh.draw();
		glEnable(GL_CULL_FACE);
		
		if (!isBrick)
		{
			glBindTexture(GL_TEXTURE_2D, seaweedTexture);
		}
		shader.setInt("_Texture", 0);
		shader.setMat4("_Model", seaweedTransform.getModelMatrix());
		seaweedMesh.draw();

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}
			}

			ImGui::ColorEdit3("BG color", &bgColor.x);
			ImGui::ColorEdit3("Light 0 Color", &light[0].color.x);
			ImGui::ColorEdit3("Light 1 Color", &light[1].color.x);
			ImGui::ColorEdit3("Light 2 Color", &light[2].color.x);
			if (ImGui::DragFloat3("Light 0", &light[0].position.x, 0.1f)) {
				shader.setVec3("_Light[0].position", light[0].position);
				lightTransform[0].position = light[0].position;
				lightShader.setMat4("_Model", lightTransform[0].getModelMatrix());
			}
			if (ImGui::DragFloat3("Light 1", &light[1].position.x, 0.1f)) {
				shader.setVec3("_Light[1].position", light[1].position);
				lightTransform[1].position = light[1].position;
				lightShader.setMat4("_Model", lightTransform[1].getModelMatrix());
			}
			if (ImGui::DragFloat3("Light 2", &light[2].position.x, 0.1f)) {
				shader.setVec3("_Light[2].position", light[2].position);
				lightTransform[2].position = light[2].position;
				lightShader.setMat4("_Model", lightTransform[2].getModelMatrix());
			}
			ImGui::SliderInt("Number of Lights", &numLights, 1, MAX_LIGHTS);
			if (ImGui::Checkbox("Blinn-Phong", &bp)) {
				shader.setBool("BP", bp);
			}
			if (ImGui::DragFloat("Ambient", &material.ambientK, 0.01f, 0.0f, 1.0f)) {
				shader.setFloat("_Material.ambientK", material.ambientK);
			}
			if (ImGui::DragFloat("Diffuse", &material.diffuseK, 0.01f, 0.0f, 1.0f)) {
				shader.setFloat("_Material.diffuseK", material.diffuseK);
			}
			if (ImGui::DragFloat("Specular", &material.specularK, 0.01f, 0.0f, 1.0f)) {
				shader.setFloat("_Material.specularK", material.specularK);
			}
			if (ImGui::DragFloat("Shininess", &material.shininess, 1.0f, 2.0f, 256.0f)) {
				shader.setFloat("_Material.shininess", material.shininess);
			}
			if (ImGui::Checkbox("Brick", &isBrick))
			{
				isBrick == true;
				if (isBrick)
				{
					bgColor = (ew::Vec3(255.0f, 111.0f, 111.0f)/255.0f);
				}
				else
				{
					bgColor = (ew::Vec3(0.0f, 188.0f, 255.0f)/255.0f);
				}
			}

			ImGui::End();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = ew::Vec3(0);
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}
