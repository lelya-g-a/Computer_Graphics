#include "common.h"
#include "Image.h"
#include "Player.h"
#include "Tiles.h"

#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <unistd.h>

// GLsizei - data type in OpenGL for sizes, 32 bits
constexpr GLsizei WINDOW_WIDTH  = 368; 
                           // 20 available cells, border and info table
constexpr GLsizei WINDOW_HEIGHT = 352; // 20 available cells and border

// Mouse contron (useless for this task)
struct InputState
{
    bool    keys[1024]{};        // Array of buttons - pressed / not pressed
    // Floating-point num in OpenGL, 32 bits
    GLfloat lastX        = 400;  // Start position 
    GLfloat lastY        = 300;  // Start position
    bool    firstMouse   = true;
    bool    captureMouse = true; // Is the mouse captured by our app or not?
    bool    capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f; // 
GLfloat lastFrame = 0.0f; // 


void OnKeyboardPressed(GLFWwindow* window, int key,    int scancode, 
                                           int action, int mode)
{
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_1:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case GLFW_KEY_2:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        default:
            if (action == GLFW_PRESS)
                Input.keys[key] = true;
            else if (action == GLFW_RELEASE)
                Input.keys[key] = false;
    }
}


void processPlayerMovement(Player &player, Image &screen)
{
    if (Input.keys[GLFW_KEY_W])
        player.ProcessInput(MovementDir::UP, screen);
    else if (Input.keys[GLFW_KEY_S])
        player.ProcessInput(MovementDir::DOWN, screen);
    
    if (Input.keys[GLFW_KEY_A])
        player.ProcessInput(MovementDir::LEFT, screen);
    else if (Input.keys[GLFW_KEY_D])
        player.ProcessInput(MovementDir::RIGHT, screen);
        
    if (Input.keys[GLFW_KEY_E])
        player.ProcessInput(MovementDir::ACTION, screen);
}


void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        Input.captureMouse = !Input.captureMouse;

    if (Input.captureMouse)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        Input.capturedMouseJustNow = true;
    }
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}


void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    if (Input.firstMouse)
    {
        Input.lastX = float(xpos);
        Input.lastY = float(ypos);
        Input.firstMouse = false;
    }

    GLfloat xoffset = float(xpos) - Input.lastX;
    GLfloat yoffset = Input.lastY - float(ypos);

    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    // ...
}


int initGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    std::cout << "Vendor: "   << glGetString(GL_VENDOR)   << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: "  << glGetString(GL_VERSION)  << std::endl;
    std::cout << "GLSL: "     << glGetString(GL_SHADING_LANGUAGE_VERSION) 
                                                          << std::endl;

    std::cout << "Controls: "              << std::endl; 
    std::cout << "Press right mouse button to capture/release mouse cursor  "
                                           << std::endl;
    std::cout << "W, A, S, D - movement  " << std::endl;
    std::cout << "press ESC to exit"       << std::endl;

    return 0;
}

int main(int argc, char** argv)
{
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 
                         "task1 base project", nullptr, nullptr);
    
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window); 

    // Reaction of the window to user actions
    glfwSetKeyCallback         (window, OnKeyboardPressed);  
    glfwSetCursorPosCallback   (window, OnMouseMove); 
    glfwSetMouseButtonCallback (window, OnMouseButtonClicked);
    glfwSetScrollCallback      (window, OnMouseScroll);

    if(initGL() != 0) 
        return -1;
    
    // Reset any OpenGL errors which could be present for some reason
    GLenum gl_error  = glGetError();
    while (gl_error != GL_NO_ERROR)
        gl_error = glGetError();
        
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

    // Creating Player
    Point  starting_pos {.x = screenBuffer.XCoord(), 
                         .y = screenBuffer.YCoord()};
    Player player       {starting_pos};

    // The coordinates of the lower left corner and the size
    glViewport   (0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  
    GL_CHECK_ERRORS;

    glClearColor (1.0f, 1.0f, 0.0f, 0.5f); 
    GL_CHECK_ERRORS;

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Measuring time intervals
        //GLfloat currentFrame = glfwGetTime();
        //deltaTime = currentFrame - lastFrame;
        //lastFrame = currentFrame;
        
        // Process events from queue
        glfwPollEvents();

        processPlayerMovement(player, screenBuffer);
        player.Draw(screenBuffer);
        
        if (player.IsLife())
        {
            sleep(1);
        }

        // Clean buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        GL_CHECK_ERRORS;

        // Draw the map
        glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, 
                     GL_UNSIGNED_BYTE, screenBuffer.Data()); 
        GL_CHECK_ERRORS;

        glfwSwapBuffers(window);
        
        int next = player.IsNewRoom();
        if (next != 0)
        {
            sleep(1);
            Image screenTmp(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
            
            for (int i = 0; i < WINDOW_HEIGHT; ++i)
            {
                for (int j = 0; j < WINDOW_WIDTH; ++j)
                {
                    screenTmp.PutPixel(j, i, 
                        screenBuffer.GetPixel(j, i));
                }
            }
            
            screenBuffer.NewRoom(next);
            Tiles tile;
            switch(screenBuffer.Type())
            {
                case 'A':
                    tile.SetPic("resources/ice.png", 1);
                    break;
                case 'B':
                    tile.SetPic("resources/fire.png", 1);
                    break;
                case 'C':
                    tile.SetPic("resources/stone.png", 1);
                    break;
                case 'D':
                    tile.SetPic("resources/grass.png", 1);
                    break;
                default:
                    break;
            }
            
            screenBuffer.ReadFile(screenBuffer.Room(), 
                                  screenBuffer.Type());
            player.SetCoords(screenBuffer.XCoord(),
                             screenBuffer.YCoord());
            
            screenTmp.PutPixels (0, 0, tile.Pic(), '.');
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
            GL_CHECK_ERRORS;

            glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, 
                         GL_UNSIGNED_BYTE, screenTmp.Data()); 
            GL_CHECK_ERRORS;

            glfwSwapBuffers(window);
            
            for (int i = 2; i < 22; ++i)
            {
                for (int j = 0; j <= i; j++)
                {
                    screenTmp.PutPixels
                        ((i * tileSize), (j * tileSize), 
                         tile.Pic(), '.');
                    screenTmp.PutPixels
                        ((j * tileSize), (i * tileSize), 
                         tile.Pic(), '.');
                         
                    for (int h = 0; h < tileSize; ++h)
                    {
                        for (int w = 0; w < tileSize; ++w)
                        {
                            if (j > 0)
                            {
                                screenTmp.PutPixel
                                    ((i - 1) * tileSize + w, 
                                     (j - 1) * tileSize + h, 
                                     screenBuffer.GetPixel(
                                     (i - 1) * tileSize + w, 
                                     (j - 1) * tileSize + h));
                                screenTmp.PutPixel
                                    ((j - 1) * tileSize + w, 
                                     (i - 1) * tileSize + h, 
                                     screenBuffer.GetPixel(
                                     (j - 1) * tileSize + w, 
                                     (i - 1) * tileSize + h));
                            }
                        }
                    }
                    
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
                    GL_CHECK_ERRORS;

                    glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, 
                                 GL_UNSIGNED_BYTE, screenTmp.Data()); 
                    GL_CHECK_ERRORS;

                    glfwSwapBuffers(window);
                }
            }
            
            
                             
            /*for (int i = 0; i < 22; ++i)
            {
                for (int j = 0; j <= i; j++)
                {
                    for (int h = 0; h < tileSize; ++h)
                        for (int w = 0; w < tileSize; ++w)
                        {
                            screenTmp.PutPixel
                                ((i * tileSize) + w, (j * tileSize) + h, 
                                 screenBuffer.GetPixel((i * tileSize) + w, (j * tileSize) + h));
                            screenTmp.PutPixel
                                ((j * tileSize) + w, (i * tileSize) + h, 
                                 screenBuffer.GetPixel((j * tileSize) + w, (i * tileSize) + h));
                        }
                    
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
                    GL_CHECK_ERRORS;

                    glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, 
                                 GL_UNSIGNED_BYTE, screenTmp.Data()); 
                    GL_CHECK_ERRORS;

                    glfwSwapBuffers(window);
                }
            }*/
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
            GL_CHECK_ERRORS;

            glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, 
                         GL_UNSIGNED_BYTE, screenBuffer.Data()); 
            GL_CHECK_ERRORS;

            glfwSwapBuffers(window);
        }
        
        
        if ((player.GetX() < 0) || (player.GetY() < 0))
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
            sleep(3);
        }
    }
    // End of game loop

    glfwTerminate(); // Destroy windows and cursors
    return 0;
}
