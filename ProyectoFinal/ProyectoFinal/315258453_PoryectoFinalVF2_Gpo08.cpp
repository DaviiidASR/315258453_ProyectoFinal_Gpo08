#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 1366, HEIGHT = 780;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(10.0f, 0.0f, 10.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float rot = 0.0f;
float rot1 = 0.0f;
float al=45.0f;

float mov = 0;
bool anim = false;
bool anim2 = false;
float mov1=0;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(0.0f, -3.5f, -3.0f);
glm::vec3 PosIni1(-9.5f, 4.6f, -2.0f);

bool active;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
float posX =PosIni.x, posY = PosIni.y, posZ = PosIni.z, posX1 = PosIni1.x, posY1 = PosIni1.y, posZ1 = PosIni1.z, rotRodIzq,rotRodDer ,rotBraDer,rotBraIzq,rotAsp, rotBal, rotRodIzq1, rotRodDer1, rotBraDer1, rotBraIzq1;

#define MAX_FRAMES 9
int i_max_steps = 190;
int i_curr_steps = 0;
//Estrucutra para definir todas las variables de frames
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ

	float posX1;
	float posY1;
	float posZ1;
	float incX1;
	float incY1;
	float incZ1;

	float rotRodIzq;
	float rotRodDer;
	float rotInc ;
	float rotInc2 ;


	float rotBraIzq;
	float rotBraDer;
	float rotInc3 ;
	float rotInc4 ;

	float rotAsp;
	float rotBal;
	float rotInc5;
	float rotInc6;

	float rotRodIzq1;
	float rotInc7;
	float rotRodDer1;
	float rotInc8;

	float rotBraIzq1;
	float rotBraDer1;
	float rotInc9;
	float rotInc10;


}FRAME;
//Declaracion de variables globales
FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			
bool play = false;
int playIndex = 0;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	//Posiciones de la matriz para los pointlight
	glm::vec3(-12.5f, 6.0f, 5.5f),
	glm::vec3(-10.0f, 1.2f, 13.3f),
	glm::vec3(9.0f, 7.0f, 16.0f),
	glm::vec3(14.5f, 1.0f, 12.0f)
};

//Con este apartado linkeamos nuestros pointlights con iluminacion dinamica, para controlarla mediante teclas
glm::vec3 LightP1;
glm::vec3 LightP2;
glm::vec3 LightP3;
glm::vec3 LightP4;

//Guarda el valor del frame que se esta reproduciendo y lo imprime
void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);
	
	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].posX1 = posX1;
	KeyFrame[FrameIndex].posY1 = posY1;
	KeyFrame[FrameIndex].posZ1 = posZ1;
	
	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].rotRodDer = rotRodDer;

	KeyFrame[FrameIndex].rotBraIzq = rotBraIzq;
	KeyFrame[FrameIndex].rotBraDer = rotBraDer;

	KeyFrame[FrameIndex].rotRodIzq1 = rotRodIzq1;
	KeyFrame[FrameIndex].rotRodDer1 = rotRodDer1;

	KeyFrame[FrameIndex].rotBraIzq1 = rotBraIzq1;
	KeyFrame[FrameIndex].rotBraDer1 = rotBraDer1;

	KeyFrame[FrameIndex].rotAsp = rotAsp;

	KeyFrame[FrameIndex].rotBal = rotBal;
	

	FrameIndex++;
}
//Se reinician los elementos de nuestro proyecto
//Nosotros para la animacion no la utilizamos, solo si deseamos hacerla ciclica
void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	posX1 = KeyFrame[0].posX1;
	posY1 = KeyFrame[0].posY1;
	posZ1 = KeyFrame[0].posZ1;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	rotRodDer = KeyFrame[0].rotRodDer;

	rotBraIzq = KeyFrame[0].rotBraIzq;
	rotBraDer = KeyFrame[0].rotBraDer;

	rotRodIzq = KeyFrame[0].rotRodIzq1;
	rotRodDer = KeyFrame[0].rotRodDer1;

	rotBraIzq = KeyFrame[0].rotBraIzq1;
	rotBraDer = KeyFrame[0].rotBraDer1;

	rotAsp = KeyFrame[0].rotAsp;

	rotBal = KeyFrame[0].rotBal;

}
//Funcion de interpolacion, nos indica en que posicion y que keyframe se esta seleccionando
void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].incX1 = (KeyFrame[playIndex + 1].posX1 - KeyFrame[playIndex].posX1) / i_max_steps;
	KeyFrame[playIndex].incY1 = (KeyFrame[playIndex + 1].posY1 - KeyFrame[playIndex].posY1) / i_max_steps;
	KeyFrame[playIndex].incZ1 = (KeyFrame[playIndex + 1].posZ1 - KeyFrame[playIndex].posZ1) / i_max_steps;
	
	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	KeyFrame[playIndex].rotInc2 = (KeyFrame[playIndex + 1].rotRodDer - KeyFrame[playIndex].rotRodDer) / i_max_steps;
	KeyFrame[playIndex].rotInc3 = (KeyFrame[playIndex + 1].rotBraIzq - KeyFrame[playIndex].rotBraIzq) / i_max_steps;
	KeyFrame[playIndex].rotInc4 = (KeyFrame[playIndex + 1].rotBraDer - KeyFrame[playIndex].rotBraDer) / i_max_steps;

	KeyFrame[playIndex].rotInc5 = (KeyFrame[playIndex + 1].rotAsp - KeyFrame[playIndex].rotAsp) / i_max_steps;

	KeyFrame[playIndex].rotInc6 = (KeyFrame[playIndex + 1].rotBal - KeyFrame[playIndex].rotBal) / i_max_steps;

	KeyFrame[playIndex].rotInc7 = (KeyFrame[playIndex + 1].rotRodIzq1 - KeyFrame[playIndex].rotRodIzq1) / i_max_steps;
	KeyFrame[playIndex].rotInc8 = (KeyFrame[playIndex + 1].rotRodDer1 - KeyFrame[playIndex].rotRodDer1) / i_max_steps;
	KeyFrame[playIndex].rotInc9 = (KeyFrame[playIndex + 1].rotBraIzq1 - KeyFrame[playIndex].rotBraIzq1) / i_max_steps;
	KeyFrame[playIndex].rotInc10 = (KeyFrame[playIndex + 1].rotBraDer1 - KeyFrame[playIndex].rotBraDer1) / i_max_steps;

}
//Funci[on que indicara la posicion de nuestro(s) modelo(s) a animar
void Sali(void) {

	KeyFrame[0].posZ = 0;
	KeyFrame[0].incX = 0;
	KeyFrame[0].incY = 0;
	KeyFrame[0].incZ = 0;

	KeyFrame[0].posX1 = 0;
	KeyFrame[0].posY1 = 0;
	KeyFrame[0].posZ1 = 0;
	KeyFrame[0].incX1 = 0;
	KeyFrame[0].incY1 = 0;
	KeyFrame[0].incZ1 = 0;
	KeyFrame[0].rotBal = 0;
	KeyFrame[0].rotInc6 = 0;

	KeyFrame[0].rotRodIzq = -20;
	KeyFrame[0].rotInc = 0;
	KeyFrame[0].rotRodDer = 20;
	KeyFrame[0].rotInc2 = 0;
	KeyFrame[0].rotBraIzq = -70;
	KeyFrame[0].rotInc3 = 0;
	KeyFrame[0].rotBraDer = -120;
	KeyFrame[0].rotInc4 = 0;
	FrameIndex=0;
	i_curr_steps=0 ;

	KeyFrame[1].posZ = -1;
	KeyFrame[1].incX = 0;
	KeyFrame[1].incY = 0;
	KeyFrame[1].incZ = 0;

	KeyFrame[1].posX1 = 0;
	KeyFrame[1].posY1 = 0;
	KeyFrame[1].posZ1 = 0;
	KeyFrame[1].incX1 = 0;
	KeyFrame[1].incY1 = 0;
	KeyFrame[1].incZ1 = 0;
	KeyFrame[1].rotBal = 130;
	KeyFrame[1].rotInc6 = 0;

	KeyFrame[1].rotRodIzq = 20;
	KeyFrame[1].rotInc = 0;
	KeyFrame[1].rotRodDer = -20;
	KeyFrame[1].rotInc2 = 0;
	KeyFrame[1].rotBraIzq = 0;
	KeyFrame[1].rotInc3 = 0;
	KeyFrame[1].rotBraDer = 120;
	KeyFrame[1].rotInc4 = 0;
	FrameIndex = 1;
	i_curr_steps = 1;

	KeyFrame[2].posZ = 1;
	KeyFrame[2].incX = 0;
	KeyFrame[2].incY = 0;
	KeyFrame[2].incZ = 0;

	KeyFrame[2].posX1 = 0;
	KeyFrame[2].posY1 = 0;
	KeyFrame[2].posZ1 = 0;
	KeyFrame[2].incX1 = 0;
	KeyFrame[2].incY1 = 0;
	KeyFrame[2].incZ1 = 0;
	KeyFrame[2].rotInc6 = 0;
	KeyFrame[2].rotBal = 0;

	KeyFrame[2].rotRodIzq = -20;
	KeyFrame[2].rotInc = 0;
	KeyFrame[2].rotRodDer = 20;
	KeyFrame[2].rotInc2 = 0;
	KeyFrame[2].rotBraIzq = -70;
	KeyFrame[2].rotInc3 = 0;
	KeyFrame[2].rotBraDer = -120;
	KeyFrame[2].rotInc4 = 0;
	FrameIndex = 2;
	i_curr_steps = 2;

	KeyFrame[3].posZ = -1;
	KeyFrame[3].incX = 0;
	KeyFrame[3].incY = 0;
	KeyFrame[3].incZ = 0;

	KeyFrame[3].posX1 = 0;
	KeyFrame[3].posY1 = 0;
	KeyFrame[3].posZ1 = 0;
	KeyFrame[3].incX1 = 0;
	KeyFrame[3].incY1 = 0;
	KeyFrame[3].incZ1 = 0;
	KeyFrame[3].rotInc6 = 0;
	KeyFrame[3].rotBal = 130;

	KeyFrame[3].rotRodIzq = 20;
	KeyFrame[3].rotInc = 0;
	KeyFrame[3].rotRodDer = -20;
	KeyFrame[3].rotInc2 =0;
	KeyFrame[3].rotBraIzq = 0;
	KeyFrame[3].rotInc3 = 0;
	KeyFrame[3].rotBraDer = 120;
	KeyFrame[3].rotInc4 = 0;
	FrameIndex = 3;
	i_curr_steps = 3;

	KeyFrame[4].posZ = 1;
	KeyFrame[4].incX = 0;
	KeyFrame[4].incY = 0;
	KeyFrame[4].incZ = 0;

	KeyFrame[4].posX1 = 0;
	KeyFrame[4].posY1 = 0;
	KeyFrame[4].posZ1 = 0;
	KeyFrame[4].incX1 = 0;
	KeyFrame[4].incY1 = 0;
	KeyFrame[4].incZ1 = 0;
	KeyFrame[4].rotInc6 = 0;
	KeyFrame[4].rotBal = 0;

	KeyFrame[4].rotRodIzq = -20;
	KeyFrame[4].rotInc = 0;
	KeyFrame[4].rotRodDer = 20;
	KeyFrame[4].rotInc2 =0;
	KeyFrame[4].rotBraIzq = -70;
	KeyFrame[4].rotInc3 = 0;
	KeyFrame[4].rotBraDer = -120;
	KeyFrame[4].rotInc4 = 0;
	FrameIndex = 4;
	i_curr_steps = 4;

	KeyFrame[5].posZ = -1;
	KeyFrame[5].incX = 0;
	KeyFrame[5].incY = 0;
	KeyFrame[5].incZ = 0;

	KeyFrame[5].posX1 = 0;
	KeyFrame[5].posY1 = 0;
	KeyFrame[5].posZ1 = 0;
	KeyFrame[5].incX1 = 0;
	KeyFrame[5].incY1 = 0;
	KeyFrame[5].incZ1 = 0;
	KeyFrame[5].rotInc6 = 0;
	KeyFrame[5].rotBal = 130;

	KeyFrame[5].rotRodIzq = 20;
	KeyFrame[5].rotInc = 0;
	KeyFrame[5].rotRodDer = -20;
	KeyFrame[5].rotInc2 = 0;
	KeyFrame[5].rotBraIzq = 0;
	KeyFrame[5].rotInc3 = 0;
	KeyFrame[5].rotBraDer = 120;
	KeyFrame[5].rotInc4 = 0;
	FrameIndex = 5;
	i_curr_steps = 5;

	KeyFrame[6].posZ = 1;
	KeyFrame[6].incX = 0;
	KeyFrame[6].incY = 0;
	KeyFrame[6].incZ = 0;

	KeyFrame[6].posX1 = 0;
	KeyFrame[6].posY1 = -5;
	KeyFrame[6].posZ1 = 15;

	KeyFrame[6].incX1 = 0;
	KeyFrame[6].incY1 = 0;
	KeyFrame[6].incZ1 = 0;
	KeyFrame[6].rotInc6 = 0;
	KeyFrame[6].rotBal = 0;

	KeyFrame[6].rotRodIzq = -20;
	KeyFrame[6].rotInc = 0;
	KeyFrame[6].rotRodDer = 20;
	KeyFrame[6].rotInc2 = 0;
	KeyFrame[6].rotBraIzq = -70;
	KeyFrame[6].rotInc3 = 0;
	KeyFrame[6].rotBraDer = -120;
	KeyFrame[6].rotInc4 = 0;
	FrameIndex = 6;
	i_curr_steps = 6;

	KeyFrame[7].posZ = -1;
	KeyFrame[7].incX = 0;
	KeyFrame[7].incY = 0;
	KeyFrame[7].incZ = 0;

	KeyFrame[7].posX1 = 0;
	KeyFrame[7].posY1 = 0;
	KeyFrame[7].posZ1 = 0;
	KeyFrame[7].incX1 = 0;
	KeyFrame[7].incY1 = 0;
	KeyFrame[7].incZ1 = 0;
	KeyFrame[7].rotInc6 = 0;
	KeyFrame[7].rotBal = 130;

	KeyFrame[7].rotRodIzq = 20;
	KeyFrame[7].rotInc = 0;
	KeyFrame[7].rotRodDer = -20;
	KeyFrame[7].rotInc2 = 0;
	KeyFrame[7].rotBraIzq = 0;
	KeyFrame[7].rotInc3 = 0;
	KeyFrame[7].rotBraDer = 120;
	KeyFrame[7].rotInc4 = 0;
	FrameIndex = 7;
	i_curr_steps = 7;

	KeyFrame[8].posZ = 1;
	KeyFrame[8].incX = 0;
	KeyFrame[8].incY = 0;
	KeyFrame[8].incZ = 0;

	KeyFrame[8].posX1 = 0;
	KeyFrame[8].posY1 = 6;
	KeyFrame[8].posZ1 = 20;
	KeyFrame[8].incX1 = 0;
	KeyFrame[8].incY1 = 0;
	KeyFrame[8].incZ1 = 0;
	KeyFrame[8].rotInc6 = 0;
	KeyFrame[8].rotBal = 0;

	KeyFrame[8].rotRodIzq = -20;
	KeyFrame[8].rotInc = 0;
	KeyFrame[8].rotRodDer = 20;
	KeyFrame[8].rotInc2 = 0;
	KeyFrame[8].rotBraIzq = -70;
	KeyFrame[8].rotInc3 = 0;
	KeyFrame[8].rotBraDer = -120;
	KeyFrame[8].rotInc4 = 0;
	FrameIndex = 8;
	i_curr_steps = 8;

	//saveFrame();
}

//funcion principal

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ProyectoFinalVF2", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	printf("%f", glfwGetTime());

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Lectura de los shaders que se utilizaran para el proyecto
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	//Declaracion de modelos del proyecto
	//Los modelos estan divididos en secciones o en del objeto mas grande al mas pequeño
	Model FachadaV5((char*)"Models/Fachada/FachadaV5/FachadaV5.obj");//Modelo Fachada principal

	//Polar WBB
	Model Polarcito((char*)"Models/Oso/Polarcito.obj");
	Model PiernaDerecha((char*)"Models/Oso/PiernaDerecha.obj");
	Model PiernaIzquierda((char*)"Models/Oso/PiernaIzquierda.obj");
	Model BrazoIzquierdoHacha((char*)"Models/Oso/BrazoIzquierdoHacha.obj");

	//Soccer
	//MujerSoccer

	/*Model SocBod((char*)"Models/Soccer/SocBod.obj");
	Model RLeg((char*)"Models/Soccer/RLeg.obj");
	Model LLeg((char*)"Models/Soccer/LLeg.obj");
	Model RArm((char*)"Models/Soccer/RArm.obj");
	Model LArm((char*)"Models/Soccer/LArm.obj");*/

	//PorteriaBalon
	Model Balon((char*)"Models/Soccer/Balon.obj");
	/*Model Net((char*)"Models/Soccer/Net.obj");*/



	//Aspiradora
	Model Aspiradora((char*)"Models/Aspiradora/Aspiradora.obj");
	Model Aspiradora1((char*)"Models/Aspiradora/Aspiradora1.obj");

	//Alfombra
	Model Alfombra((char*)"Models/Alfombra/Alfombra.obj");

	//Cama
	Model Cama((char*)"Models/Cama/Bedy.obj");
	Model Minion((char*)"Models/Minion/Minion.obj");
	Model MiOjoMov((char*)"Models/Minion/MiOjoMov.obj");
	Model Star((char*)"Models/Estrella/Star.obj");
	Model Cat((char*)"Models/Gato/Cat.obj");

	//Librero
	Model Librero((char*)"Models/Librero/Librero.obj");
	Model Panecitos((char*)"Models/Panecitos/Panecitos.obj");

	//Mesa 
	Model Pc((char*)"Models/lap/lap.obj");
	Model SillaArriba((char*)"Models/Silla/SillaArriba.obj");
	Model SillaAbajo((char*)"Models/Silla/SillaAbajo.obj");
	Model Mesa((char*)"Models/Mesa/mesaD.obj");

	//Buro
	Model Lamp((char*)"Models/Lamp/Lamp.obj");
	Model Buro((char*)"Models/Buro/Buro.obj");
	Model CajonBuro((char*)"Models/Buro/Cajon.obj");

	//Revistas
	Model Revista2C((char*)"Models/Revistas/Revista2C.obj");
	Model RevistaS((char*)"Models/Revistas/RevistaS.obj");
	Model RevistaA((char*)"Models/Revistas/3Revistas.obj");


	//Mesa Tocadiscos
	Model Desk((char*)"Models/Mueble/desk.obj");
	Model Tocadiscos((char*)"Models/Tocadiscos/Tocadiscos.obj");
	Model Pluma((char*)"Models/Tocadiscos/PlumaT.obj");
	Model Acetato((char*)"Models/Tocadiscos/Acetato.obj");

	// Build and compile our shader program

	//Inicialización de KeyFrames
	/*Este apartado sirve para realizar uno la animacion, al gusto del usuario
	Si se desea animarlo por parte del usuario, es necesario mover este apartado a la funcion voidSali(void)
	y comentar la inicializacion de los KeyFrames en ese apartado para inicializarlos nosotros*/
	/*for(int i=0; i<MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].rotInc = 0;
		KeyFrame[i].rotRodDer = 0;
		KeyFrame[i].rotInc2 = 0;
		KeyFrame[i].rotBraIzq = 0;
		KeyFrame[i].rotInc3 = 0;
		KeyFrame[i].rotBraDer = 0;
		KeyFrame[i].rotInc4 = 0;
	}*/




	// Se dibujan y se posicionan normales de los modelos, la posicion de las texturas y como se dibujara el modelo, ojo, no es la posicion del modelo
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	//Posicion de las caras del skybox
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	//indices para dibujar
	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);


	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Lectura de Archivos para Skybox
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();


		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Asignamos valores nuestro foco, luces y lampara
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), LightP2.x, LightP2.y, LightP2.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), LightP2.x, LightP2.y, LightP2.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), LightP3.x, LightP3.y, LightP3.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), LightP3.x, LightP3.y, LightP3.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), LightP4.x, LightP4.y, LightP4.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), LightP4.x, LightP4.y, LightP4.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();


		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind diffuse map
		//glBindTexture(GL_TEXTURE_2D, texture1);*/

		// Bind specular map
		/*glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);*/


		glBindVertexArray(VAO);
		glm::mat4 tmp = glm::mat4(1.0f); //Temp



		//Carga de modelos 
		//Personaje
		//Cuerpo de polar
		view = camera.GetViewMatrix();
		glm::mat4 model(1);
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		tmp = model = glm::translate(model, glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Polarcito.Draw(lightingShader);

		//Pierna Derecha
		view = camera.GetViewMatrix();
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::translate(tmp, glm::vec3(1.3f, 0.4f, 0.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PiernaDerecha.Draw(lightingShader);

		//Pierna Derecha
		view = camera.GetViewMatrix();
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::translate(tmp, glm::vec3(-1.3f, 0.4f, 0.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PiernaIzquierda.Draw(lightingShader);

		//Brazo derecho
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.675f, 3.43f, 0.5f));
		model = glm::rotate(model, glm::radians(-rotBraDer), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		BrazoIzquierdoHacha.Draw(lightingShader);

		//Soccer
		//Mujer Soccer
		//Cuerpo

		/*model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(posX1, posY1, posZ1));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodDer1), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SocBod.Draw(lightingShader);*/

		//Brazo derecho

		/*view = camera.GetViewMatrix();
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(posX1, posY1, posZ1));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodDer1), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RArm.Draw(lightingShader);*/

		//Brazo Izquierdo

		/*view = camera.GetViewMatrix();
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(posX1, posY1, posZ1));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodDer1), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LArm.Draw(lightingShader);*/

		//Pierna Derecha

		/*view = camera.GetViewMatrix();
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(posX1, posY1, posZ1));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodDer1), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RLeg.Draw(lightingShader);*/

		//Pierna Izquierda

		/*model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(posX1, posY1, posZ1));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodDer1), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		LLeg.Draw(lightingShader);*/

		//Balon
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::translate(model, glm::vec3(posX1, posY1, posZ1));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-rotBal), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Balon.Draw(lightingShader);

		//Porteria

		/*model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(posX1, posY1, posZ1));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-rotRodDer1), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Net.Draw(lightingShader);*/

		//Fachada

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(5.5f, 5.5f, 5.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f, -0.8f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		FachadaV5.Draw(lightingShader);

		//Aspiradora

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::translate(model, glm::vec3(0.15f, -1.7f, 1.25f));
		model = glm::rotate(model, glm::radians(-rotAsp), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Aspiradora.Draw(lightingShader);

		/*model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::translate(model, glm::vec3(0.15f, -1.7f, 1.25f));
		model = glm::rotate(model, glm::radians(-rotAsp), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Aspiradora1.Draw(lightingShader);*/

		//Alfombra

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(5.5f, 3.5f, 3.5f));
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Alfombra.Draw(lightingShader);

		//cama

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -3.6f, 15.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cama.Draw(lightingShader);

		//Gatito

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-2.5f, -1.0f, 11.7f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cat.Draw(lightingShader);

		//Estrella

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.5f, -1.0f, 11.7f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Star.Draw(lightingShader);

		// Minion

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.7f, 5.7f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Minion.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.7f, 5.7f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rot1), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		MiOjoMov.Draw(lightingShader);

		//Libero

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(3.8f, 3.8f, 3.8f));
		model = glm::translate(model, glm::vec3(-4.0f, -1.0f, -1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Librero.Draw(lightingShader);

		//Panecitos

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		model = glm::translate(model, glm::vec3(-9.0f, -2.5f, 0.5f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Panecitos.Draw(lightingShader);


		//Laptop

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(6.0f, 0.175f, 5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pc.Draw(lightingShader);

		//Silla

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::translate(model, glm::vec3(5.5f, -0.75f, 6.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rot1), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaArriba.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::translate(model, glm::vec3(5.5f, -0.75f, 6.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rot1), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		SillaAbajo.Draw(lightingShader);

		//Mesa

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(6.0f, -1.375f, 5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mesa.Draw(lightingShader);

		//Lapmara

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(-3.3f, -0.225f, 4.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Lamp.Draw(lightingShader);

		//Buro

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(-3.3f, -0.8f, 5.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Buro.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::translate(model, glm::vec3(-3.3f, -0.8f, 5.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, mov));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CajonBuro.Draw(lightingShader);

		//Revistas
		//Revista Abierta

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::translate(model, glm::vec3(17.0f, -14.0f, -7.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Revista2C.Draw(lightingShader);

		//Revista Cerrada

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		model = glm::translate(model, glm::vec3(-6.0f, -4.1f, -2.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RevistaS.Draw(lightingShader);

		//Revista Amontonada

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::translate(model, glm::vec3(0.8f, -0.85f, 0.8f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RevistaA.Draw(lightingShader);

		//Mueble

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::translate(model, glm::vec3(3.6f, -0.875f, 0.85f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Desk.Draw(lightingShader);

		//Tocadiscos

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(5.85f, 0.16f, 1.2f));
		model = glm::rotate(model, glm::radians(rot1), glm::vec3(0.0f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pluma.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(5.5f, 0.1f, 1.6f));
		model = glm::rotate(model, glm::radians(rot1), glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Acetato.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::translate(model, glm::vec3(5.5f, 0.1f, 1.6f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Tocadiscos.Draw(lightingShader);

		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);//con esta función apagamos el shader que estemos usando para poder usar uno nuevo, y este no desapárezca y halla errores


		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxshader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default




		// Swap the screen buffers
		glfwSwapBuffers(window);
	}




	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();




	return 0;
}

//animacion del personaje, como nosotros podemos animarlo manualmente
void animacion()
{

		//Movimiento del personaje

		if (play)
		{
			if (i_curr_steps >= i_max_steps) //end of animation between frames?
			{
				playIndex++;
				if (playIndex>FrameIndex - 2)	//end of total animation?
				{
					printf("termina anim\n");
					playIndex = 0;
					play = false;
				}
				else //Next frame interpolations
				{
					i_curr_steps = 0; //Reset counter
									  //Interpolation
					interpolation();
				}
			}
			else
			{
				//Draw animation
				posX += KeyFrame[playIndex].incX;
				posY += KeyFrame[playIndex].incY;
				posZ += KeyFrame[playIndex].incZ;

				posX1 += KeyFrame[playIndex].incX1;
				posY1 += KeyFrame[playIndex].incY1;
				posZ1 += KeyFrame[playIndex].incZ1;

				rotRodIzq += KeyFrame[playIndex].rotInc;
				rotRodDer += KeyFrame[playIndex].rotInc2;
				rotBraDer += KeyFrame[playIndex].rotInc3;
				rotBraIzq += KeyFrame[playIndex].rotInc4;

				rotBal += KeyFrame[playIndex].rotInc6;

				i_curr_steps++;
			}

		}
	}


// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	//Reproducción de animaciones y acciones de nuestros modelos via teclado
	
	if (keys[GLFW_KEY_L])
	{
		if (play == false && (FrameIndex >= 0))
		{
						
			interpolation();
			play = true;
			Sali();
			playIndex = 0;
			i_curr_steps = 0;
			

		}/*
		else
		{
			play = false;
		}*/

	}

	if (keys[GLFW_KEY_K])
	{
		if (FrameIndex<MAX_FRAMES)
		{
			saveFrame();
		}

	}
	//Animaciones secillas se reproducen con la Tecla "0", -ojo NO es cero-
	if (keys[GLFW_KEY_O]) {
		anim = true;

	}

	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
	//Activamos la iluminación de nuestros pointlight
	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	if (keys[GLFW_KEY_Z])
	{
		active = !active;
		if (active)
			LightP2 = glm::vec3(1.0f, 1.0f, 1.0f);
		else
			LightP2 = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	if (keys[GLFW_KEY_X])
	{
		active = !active;
		if (active)
			LightP3 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP3 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	if (keys[GLFW_KEY_C])
	{
		active = !active;
		if (active)
			LightP4 = glm::vec3(1.0f, 1.0f, 0.0f);
		else
			LightP4 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
	//animación sencilla, que es lo que hace
	if (anim)
	{
		if (rot1 < 45.0f && mov < 0.95f) {
			rot1 += 0.4f;
			mov += 0.002f;

		}
		else {
			anim = false;
			anim2 = true;
		}
	}
	if (anim2)
	{
		if (rot1 > 15.0f && mov > 0.0f) {
			rot1 -= 0.4f;
			mov -= 0.002f;
		}

		else {
			anim2 = false;
			anim = true;
		}
	}
	//movimiento manual de los modelos que esten asociadas con esta bariable rot
	if (keys[GLFW_KEY_1])
	{

		rot += 1;

	}

	if (keys[GLFW_KEY_2])
	{
		if (rotRodIzq<80.0f)
			rotRodIzq += 1.0f;
			
	}

	if (keys[GLFW_KEY_3])
	{
		if (rotRodIzq>-45)
			rotRodIzq -= 1.0f;
		
	}
	if (keys[GLFW_KEY_4])
	{
		if (rotRodDer < 80.0f)
			rotRodDer += 1.0f;

	}
	if (keys[GLFW_KEY_5])
	{
		if (rotRodDer > -45)
			rotRodDer -= 1.0f;

	}

	if (keys[GLFW_KEY_6])
	{
		if (rotBraDer > -45)
			rotBraDer -= 1.0f;

	}
	if (keys[GLFW_KEY_7])
	{
		if (rotBraDer < 80.0f)
			rotBraDer += 1.0f;

	}
	if (keys[GLFW_KEY_8])
	{
		if (rotBraIzq < 80.0f)
			rotBraIzq += 1.0f;

	}

	if (keys[GLFW_KEY_9])
	{
		if (rotBraIzq > -45)
			rotBraIzq -= 1.0f;

	}

	if (keys[GLFW_KEY_0])
	{
		if (rotBraDer > -45)
			rotBraDer -= 1.0f;

	}

	

	//Mov Personaje
	if (keys[GLFW_KEY_H])
	{
		posZ += 1;
	}

	if (keys[GLFW_KEY_Y])
	{
		posZ -= 1;
	}

	if (keys[GLFW_KEY_G])
	{
		posX -= 1;
	}

	if (keys[GLFW_KEY_J])
	{
		posX += 1;
	}




	// Camera controls
	//Movimiento de la camara sintetica a travez del uso cotidiano de las teclas WASD
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}






}