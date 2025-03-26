#include "Window.h"
#include <System.h>
#include <chrono>

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";
double prevTime = 0;
double currTime = 0;

// Objects to render
System* sys;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

bool Window::initializeObjects() {
    // Create a cube
    //cube = new Cube();
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));
    sys = new System();
    sys->InitSimpleParticleSystem();
    sys->Update(1.0 / 60.0);


    return true;
}

void Window::cleanUp() {
    // Deallcoate the objects.
    //delete cube;

    // Delete the shader program.
    glDeleteProgram(shaderProgram);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Create the GLFW window.
    GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // set up the camera
    Cam = new Camera();
    Cam->SetAspect(float(width) / float(height));

    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    Cam->SetAspect(float(width) / float(height));
}

bool flag = false;

// update and draw functions
void Window::idleCallback() {
    // Perform any updates as necessary.
    Cam->Update();

    std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration<double, std::milli>(now.time_since_epoch()).count();

    prevTime = currTime;
    currTime = time;

    if (currTime != 0 && prevTime != 0) {
        double deltaTime = currTime - prevTime;
        sys->Update(deltaTime);

        if (!flag) {
            
            flag = true;
        }
    }


}

void Window::displayCallback(GLFWwindow* window) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sys->Draw(Cam->GetViewProjectMtx(), Window::shaderProgram);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Particle Simulation");
    // Text that appears in the window
    ImGui::Text("Simulation Settings");
    // Checkbox that appears in the window
    //ImGui::Checkbox("Draw Triangle", &drawTriangle);
    // Slider that appears in the window
    ImGui::SliderFloat("Spawn Rate", &(sys->creationrate), 0.5f, 2000.0f);
    ImGui::SliderFloat("Lifetime", &(sys->particlelifetime), 200.0f, 10000.0f);
    ImGui::SliderFloat("Lifetime Var.", &(sys->particlelifetimevar), 0.01f, 100000000.0f);
    ImGui::SliderFloat("Ground Level", &(sys->ground), -10.0f, 10.0f);
    ImGui::SliderFloat("Gravity", &(sys->gravity), 0.1f, 10.0f);
    ImGui::SliderFloat("Air Density", &(sys->airdensity), 0.5f, 10.0f);
    ImGui::SliderFloat("G. Friction", &(sys->friction), 0.0f, 1.0f);
    ImGui::SliderFloat("Elasticity", &(sys->particleElasticity), 0.0f, 1.0f);

    ImGui::SliderFloat("Init pos x", &(sys->initpos.x), -10.0f, 10.0f);
    ImGui::SliderFloat("Init pos y", &(sys->initpos.y), -10.0f, 10.0f);
    ImGui::SliderFloat("Init pos z", &(sys->initpos.z), -10.0f, 10.0f);

    ImGui::SliderFloat("Init pos var x", &(sys->initposvar.x), 0.01f, 25.0f);
    ImGui::SliderFloat("Init pos var y", &(sys->initposvar.y), 0.01f, 25.0f);
    ImGui::SliderFloat("Init pos var z", &(sys->initposvar.z), 0.01f, 25.0f);

    ImGui::SliderFloat("Init vel x", &(sys->initvel.x), -10.0f, 10.0f);
    ImGui::SliderFloat("Init vel y", &(sys->initvel.y), -10.0f, 10.0f);
    ImGui::SliderFloat("Init vel z", &(sys->initvel.z), -10.0f, 10.0f);

    ImGui::SliderFloat("Init vel var x", &(sys->initposvar.x), 0.01f, 25.0f);
    ImGui::SliderFloat("Init vel var y", &(sys->initposvar.y), 0.01f, 25.0f);
    ImGui::SliderFloat("Init vel var z", &(sys->initposvar.z), 0.01f, 25.0f);
    // Fancy color editor that appears in the window
    //ImGui::ColorEdit4("Color", color);
    // Ends the window
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Gets events, including input such as keyboard and mouse or window resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
    Cam->SetAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    /*
     * TODO: Modify below to add your key callbacks.
     */

    // Check for a key press.
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                // Close the window. This causes the program to also terminate.
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_R:
                resetCamera();
                break;

            default:
                break;
        }
    }
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        LeftDown = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        RightDown = (action == GLFW_PRESS);
    }
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
    int maxDelta = 100;
    int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
    int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

    MouseX = (int)currX;
    MouseY = (int)currY;

    // Move camera
    // NOTE: this should really be part of Camera::Update()
    if (LeftDown) {
        const float rate = 1.0f;
        Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
        Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
    }
    if (RightDown) {
        const float rate = 0.005f;
        float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
        Cam->SetDistance(dist);
    }
}