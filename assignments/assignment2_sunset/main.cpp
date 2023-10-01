#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <bh/shader.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

unsigned int createShader(GLenum shaderType, const char* sourceCode);
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
unsigned int createVAO(float* vertexData, int numVertices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

struct Vertex {
	float x, y, z;
	float u, v;
};

Vertex vertices[4] = {
	//x       y     z     u       v
   { -0.5f, -0.5f, 0.0f, 0.0f , 0.0f }, //Bottom left
   { 0.5f, -0.5f,  0.0f, 1.0f , 0.0f }, //Bottom right
   { 0.5f,  0.5f, 0.0f, 1.0f , 1.0f },  //Top right
   { -0.5f,  0.5f, 0.0f, 0.0f , 1.0f }  //Top left
};


unsigned int indices[6] = {
	0 , 1 , 2 , //Triangle 1
	2 , 3 , 0  //Triangle 2
};

float triangleColor[3] = { 1.0f, 0.5f, 0.0f };
float triangleBrightness = 1.0f;
bool showImGUIDemoWindow = true;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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

	bh::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	shader.use();
	shader.setFloat("_MyFloat", 1.0f);
	shader.setVec2("_Vec2", indices[0], indices[1]);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.setVec3("skyColorBottom", 0.85f, 0.1f, 0.25f);
		shader.setVec3("skyColorTop", 1.0f, 0.0f, 0.0f);
		shader.setVec3("sunColor", 1.0f, 1.0f, 0.0f);
		float sunRadius = 0.2f;
		float sunSpeed = 1.0f;
		shader.setVec3("hillColor", 0.0f, 0.5f, 0.0f);


		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			//Wireframe
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			//Shaded
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//Draw using indices
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			/*ImGui::ColorEdit3("Sky Color Bottom", &skyColorBottom.x);
			ImGui::ColorEdit3("Sky Color Top", &skyColorTop.x);
			ImGui::ColorEdit3("Sun Color", &sunColor.x);
			ImGui::SliderFloat("Sun Radius", &sunRadius, 0.0f, 1.0f);
			ImGui::SliderFloat("Sun Speed", &sunSpeed, 0.0f, 2.0f);
			ImGui::ColorEdit3("Hill Color", &hillColor.x);

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Color", triangleColor);
			ImGui::SliderFloat("Brightness", &triangleBrightness, 0.0f, 1.0f);
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}*/

			// for some reason the above code does not work no matter what i do but i'll leave it here for the effort

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices)
{
	unsigned int vao = createVAO(vertices, 4, indices, 6);
	glBindVertexArray(vao);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, vertexData, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	// UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, u));
	glEnableVertexAttribArray(1);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

