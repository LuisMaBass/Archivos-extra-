/*---------------------------------------------------------*/
/* ----------------  Proyecto Laboratorio EXtra -----------*/
/*-----------------    2024-1   ---------------------------*/
/*----------- Alumno:  Macias Bassoco Juan Luis -----------*/
/*------------- No. Cuenta  315220520       ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//ma
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(200.0f, 30.0f, 300.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS =60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

//// Light
glm::vec3 lightColor = glm::vec3(0.7f);
glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

// posiciones


float	movAnilloazul_x = 0.0f,
		movAnilloazul_z = 0.0f,
		movAnilloazul_y = 0.0f,
		movAnilloverde_x = 0.0f,
		movAnilloverde_z = 0.0f,
		movAnilloverde_y = 0.0f,
		movAnillorojo_x = 0.0f,
		movAnillorojo_z = 0.0f,
		movAnillorojo_y = 0.0f;
bool	animacion0 = false;
int		pivote = 0;		


#define MAX_FRAMES 19  // espacio en memoria que se va a calcular 
int		i_max_steps = 60; // cantidad de cuadros intermediso que el diagrama va a calcular 
int		i_curr_steps = 0;
	
typedef struct _frame
{
	//Variables para GUARDAR Key Frames se acede mediante el arreglo 
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;
	float giroBrazoDer;	//variable para poder guardar
	float giroBrazoIzq; //brazo derecho
	float rotRodDer;	//rodilla derecha
	float giroCabeza;	// rotacion de cabeza 




}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 2;			//introducir número en caso de tener Key guardados
bool play = false;
int playIndex = 0;

void animate(void)
{

	//Vehículo
	if (animacion0){
		//======================= paso 1 ======================
	//sube  base 1 anillo azul 
		if (pivote  == 0) {
			movAnilloazul_y += 1.5f;
		
			if (movAnilloazul_y >= 15.0f) {
				pivote = 1;
			}
		}// mueve a base 3 
		if (pivote == 1) {
			movAnilloazul_x += 1.5f;
			
			if (movAnilloazul_x >= 210.0f) {
				pivote = 2;
			}
		}// baja tope base 3 
		if (pivote == 2) {
			movAnilloazul_y -= 3.0f;
		
			if (movAnilloazul_y <= -24.0f)
				pivote = 3;
			//======================= paso 2 ======================
		}//sube base 1 anillo verde
		if (pivote == 3) {
			movAnilloverde_y += 1.5f;
			
			if (movAnilloverde_y >= 20.0f)
				pivote = 4;
		}// mueve base 2 
		if (pivote == 4) {
			movAnilloverde_x += 1.5f;
			
			if (movAnilloverde_x >= 107.0f) {
				pivote = 5;
				//animacion = false;
			}
		}// baja base 2 
		if (pivote == 5) {
			movAnilloverde_y -= 1.5f;
			
		
			if (movAnilloverde_y <= -13.0f)
				pivote = 6;
			
		}
		//======================= paso 3 ======================
		// mueve azul de base 3 a base 2 
		//sube 
		if (pivote == 6) {
			movAnilloazul_y += 1.5f;
		
			if (movAnilloazul_y >= 25.0f)
				pivote = 7;
		}// mueve a base 2
		if (pivote == 7) {
			movAnilloazul_x -= 1.5f;
			if (movAnilloazul_x <= 107.5f)
				pivote = 8;
		}// baja tope base 2
		if (pivote == 8) {
			movAnilloazul_y -= 1.5f;
		
			if (movAnilloazul_y <= -15.0f)
				pivote = 9;
			//======================= paso 4 ======================	
			//anillo rojo de base 1 a base 3 
		}//sube base 1 anillo rojo 
		if (pivote == 9) {
			movAnillorojo_y += 1.5f;
			if (movAnillorojo_y >= 40.0f)
				pivote = 10;
		}// mueve base 3 
		if (pivote == 10) {
			movAnillorojo_x += 1.5f;
			if (movAnillorojo_x >= 210.0f) 
				pivote = 11;
		}// baja base 2 
		if (pivote == 11) {
			movAnillorojo_y -= 1.5f;
			if (movAnillorojo_y <= 0.0f)
				pivote = 12;
		}
		//======================= paso 5 ======================	
		//anillo azul de base 2 a base 1 
	//sube base 1 anillo azul en base 2 
	if (pivote == 12) {
		movAnilloazul_y += 1.5f;
		
		if (movAnilloazul_y >= 20.0f)
			pivote = 13;
	}// mueve base 1 
	if (pivote == 13) {
		movAnilloazul_x -= 1.5f;
		
		if (movAnilloazul_x <= 3.0f)
			pivote = 14;
	}// baja base 2 
	if (pivote == 14) {
		movAnilloazul_y -= 1.5f;

		if (movAnilloazul_y <= -24.0f)
			pivote = 15;
	}
		//======================= paso 6 ======================	
		//anillo verde de base 2 a base 3 
		//sube de base 2 anillo verde 
		if (pivote == 15) {
			movAnilloverde_y += 1.5f;
			
			if (movAnilloverde_y >= 40.0f)
				pivote = 16;
		}// mueve base 3 
		if (pivote == 16) {
			movAnilloverde_x += 1.5f;
			
			if (movAnilloverde_x >= 210.0f)
				pivote = 17;
		}// baja base 3 
		if (pivote == 17) {
			movAnilloverde_y -= 1.5f;

			
			if (movAnilloverde_y <= 0.0f)
				pivote = 18;
		}
		//======================= paso 7 ======================	
		//anillo azul de bas 1 a base 3 
		//sube de base 1 anilo azul 
		if (pivote == 18) {
			movAnilloazul_y += 1.5f;
			
			if (movAnilloazul_y >= 40.0f)
				pivote = 19;
		}// mueve de base 1 a base 3 
		if (pivote == 19) {
			movAnilloazul_x += 1.5f;
			
			if (movAnilloazul_x >= 210.0f)
				pivote = 20;
			//animacion = false;
		}// baja base 3
		if (pivote == 20) {
			movAnilloazul_y -= 1.5f;	
			if (movAnilloazul_y <= 0.0f)
				animacion0 = false;
		}
	}
}


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}
int main()
{

	glfwInit();

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGEIHC Macias Bassoco Juan Luis", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glEnable(GL_DEPTH_TEST);

	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg",
		
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model base("resources/objects/figuras/Basep.obj");
	Model anillorojo("resources/objects/figuras/anillorojop.obj");
	Model anilloverde("resources/objects/figuras/anilloverdep.obj");
	Model anilloazul("resources/objects/figuras/anilloazulp.obj");
	Model plano("resources/objects/figuras/planop.obj");

	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		lastFrame = SDL_GetTicks();

	
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", ambientColor);
		staticShader.setVec3("dirLight.diffuse", diffuseColor);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.6f, 0.6f, 0.6f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		//============== no se mueve. =============
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		base.Draw(staticShader); 
		//========== anillo azul ===================
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, -70.0f));
		model = glm::translate(model, glm::vec3(movAnilloazul_x, movAnilloazul_y, movAnilloazul_z));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		anilloazul.Draw(staticShader);
		//========== anillo Verde ===================
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, -70.0f));
		model = glm::translate(model, glm::vec3( movAnilloverde_x, movAnilloverde_y, movAnilloverde_z));// declarar mi movimiento
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		anilloverde.Draw(staticShader);
		//========== anillo rojo  ===================
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, -70.0f));
		model = glm::translate(model, glm::vec3(movAnillorojo_x, movAnillorojo_y, movAnillorojo_z));// declarar mi movimiento
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		anillorojo.Draw(staticShader);
		//========== Plano con Nombre   ===================
		model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 0.0f, -50.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));
		plano.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	skybox.Terminate();
	glfwTerminate();
	return 0;
}
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		animacion0 ^= true;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) { 
		movAnilloazul_x = 0.0f;
		movAnilloazul_z = 0.0f;
		movAnilloazul_y = 0.0f;
		movAnilloverde_x = 0.0f;
		movAnilloverde_z = 0.0f;
		movAnilloverde_y = 0.0f;
		movAnillorojo_x = 0.0f;
		movAnillorojo_z = 0.0f;
		movAnillorojo_y = 0.0f;
		pivote = 0;
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
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}