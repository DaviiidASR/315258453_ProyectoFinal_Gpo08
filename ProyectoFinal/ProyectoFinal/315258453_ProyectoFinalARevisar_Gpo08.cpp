// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
// Properties
const GLuint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
Camera camera( glm::vec3( 0.0f, 2.0f, 25.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
float mov = 0.0f;
float mov1 = 0.0f;
bool anim = false;
bool anim2 = false;


int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "ProyFinalv2", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shaders/modelLoading.vs", "Shaders/modelLoading.frag" );
    Shader lampshader( "Shaders/lamp.vs", "Shaders/lamp.frag" );

    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
    GLfloat vertices[] =
    {

        //Coordenadas de textura
        // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 

    };

    GLuint indices[] =
    {  // Note that we start from 0!
        0,1,3,
        1,2,3

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
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Load textures

    Model Fachada((char*)"Models/Fachada/FachadaV1/Fachada.obj");
    //Model FachadaV2((char*)"Models/Fachada/FachadaV2.obj");
    Model Alfombra((char*)"Models/Alfombra/Alfombra.obj");
    Model Aspiradora((char*)"Models/Aspiradora/Aspiradora.obj");
    Model Pardo((char*)"Models/Oso/Pardo.obj");
    Model Libro((char*)"Models/Book/Booky2.obj");
    Model Cama((char*)"Models/Cama/Bedy.obj");
    Model Minion((char*)"Models/Minion/Minion.obj");
    Model MiOjoMov((char*)"Models/Minion/MiOjoMov.obj");
    Model Desk((char*)"Models/Mueble/desk.obj");
    Model Estante((char*)"Models/Mueble/Estante.obj");
    Model Star((char*)"Models/PEstrella/Pestrella1.obj");
    Model Cat((char*)"Models/Gato/gatito.obj");
    Model Lamp((char*)"Models/Lamp/Lamp.obj");
    Model Pc((char*)"Models/lap/lap.obj");
    Model SillaArriba((char*)"Models/Silla/SillaArriba.obj");
    Model SillaAbajo((char*)"Models/Silla/SillaAbajo.obj");
    Model Tocadiscos((char*)"Models/Tocadiscos/Tocadiscos.obj");
    Model Pluma((char*)"Models/Tocadiscos/PlumaT.obj");
    Model Acetato((char*)"Models/Tocadiscos/Acetato.obj");
    Model Mesa((char*)"Models/Mesa/mesaD.obj");
    Model Buro((char*)"Models/Buro/Buro.obj");
    Model CajonBuro((char*)"Models/Buro/Cajon.obj");

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
  
    image = stbi_load("images/BookyT.png", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        //pardo
        glm::mat4 model(1);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Pardo.Draw(shader);
        
        //Fachada

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.002f));
        //model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Fachada.Draw(shader);

        //Aspiradora

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.0f,2.0f,2.0f));
        model = glm::translate(model,glm::vec3(0.0f,-1.75f,1.0f));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f,1.0f,0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Aspiradora.Draw(shader);

        //Alfombra

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(3.5f,3.5f, 3.5f));
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Alfombra.Draw(shader);
        
        //Minion

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.75f, 5.7f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Minion.Draw(shader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.75f, 5.7f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        MiOjoMov.Draw(shader);

        //cama
        //La cama tiene el pivote al pie de la misma, se intenta hacer que un peluche la levante doblando sus brazos
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -3.5f, 15.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Cama.Draw(shader);

        //Estante
        //se plantea jugar con los libros haciendo que se caigan y vuelvana a su posicion
        model = glm::mat4(1);       
        model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
        model = glm::translate(model, glm::vec3(-3.0f, -1.0f, -1.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Estante.Draw(shader);

        //Mueble
        //Se preveee jugar con los cajones, para que se puedan meter y sacar
        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
        model = glm::translate(model, glm::vec3(2.6f, -0.875f, 0.85f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Desk.Draw(shader);
        
        //Peluche Estrella

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        model = glm::translate(model, glm::vec3(1.0f, -0.38f, 4.75f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Star.Draw(shader);

        //Peluche Gato

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        model = glm::translate(model, glm::vec3(-1.3f, -0.37f, 4.40f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Cat.Draw(shader);

        //Lapmara

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        model = glm::translate(model, glm::vec3(-2.3f, -0.15f, 5.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Lamp.Draw(shader);

        //Buro

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        model = glm::translate(model, glm::vec3(-2.3f, -0.75f, 5.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Buro.Draw(shader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        model = glm::translate(model, glm::vec3(-2.3f, -0.75f, 5.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f,0.0f,mov));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        CajonBuro.Draw(shader);

        //Laptop

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        model = glm::translate(model, glm::vec3(4.5f, 0.175f, 5.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Pc.Draw(shader);

        //Silla

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        model = glm::translate(model, glm::vec3(3.0f, -0.75f, 6.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        SillaArriba.Draw(shader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        model = glm::translate(model, glm::vec3(3.0f, -0.75f, 6.0f));  
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        SillaAbajo.Draw(shader);

        //Mesa

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        model = glm::translate(model, glm::vec3(4.5f, -1.375f, 5.0f));
        //model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Mesa.Draw(shader);

        //Tocadiscos

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        model = glm::translate(model, glm::vec3(4.35f, 0.16f, 1.2f));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, -0.5f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Pluma.Draw(shader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        model = glm::translate(model, glm::vec3(4.0f, 0.1f, 1.6f));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, -1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Acetato.Draw(shader);

        model = glm::mat4(1);
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        model = glm::translate(model, glm::vec3(4.0f, 0.1f, 1.6f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Tocadiscos.Draw(shader);

        //libro
        //Problema con la textura, se pone la imagen, cuando elimino la parte de arriba de incluir la imagen
        //desaparece el moedlo del libro
        model = glm::mat4(1);     
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f)); 
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 6, GL_FLAT, 0);
        Libro.Draw(shader);

        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        lampshader.Use();


        //glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }


    /*if (keys[GLFW_KEY_O])
    {
        rot += 0.5f;
    }*/

    if (anim)
    {
        if (rot < 90.0f && mov < 0.95f ){
            rot += 0.3f;
            mov += 0.002f;
            
        }
        else {
            anim = false;
            anim2 = true;
        }
    }
    if (anim2)
    {
        if (rot > 0.0f && mov > 0.0f ) {
            rot -= 0.3f;
            mov -= 0.002f;
        }
            
        else {
            anim2 = false;
            anim = true;
        }
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_O]) {
        anim = true;

    }

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

