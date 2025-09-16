using namespace std;
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "dice.h"
#include "button.h"

#define WIN_HEIGHT 600
#define WIN_WIDTH 600

enum class State{
    D4,
    D6,
    D8,
    ROLL,
    D10,
    D12,
    D20,
    CUSTOM
};

std::string stateToString(State s) {
    switch(s) {
        case State::D4:  return "D4";
        case State::D6:  return "D6";
        case State::D8:  return "D8";
        case State::ROLL:  return "ROLL";
        case State::D10: return "D10";
        case State::D12: return "D12";
        case State::D20: return "D20";
        case State::CUSTOM:  return "CUSTOM";
    }
    return "Unknown";
}


GLuint loadShaderProgram(const std::string& vertexPath, const std::string& fragmentPath) {
    // 1. Read vertex shader
    std::ifstream vFile(vertexPath);
    if (!vFile.is_open()) {
        std::cerr << "Failed to open vertex shader: " << vertexPath << "\n";
        exit(-1);
    }
    std::stringstream vBuffer;
    vBuffer << vFile.rdbuf();
    std::string vertexCodeStr = vBuffer.str();
    const char* vertexCode = vertexCodeStr.c_str();
    vFile.close();

    // 2. Read fragment shader
    std::ifstream fFile(fragmentPath);
    if (!fFile.is_open()) {
        std::cerr << "Failed to open fragment shader: " << fragmentPath << "\n";
        exit(-1);
    }
    std::stringstream fBuffer;
    fBuffer << fFile.rdbuf();
    std::string fragmentCodeStr = fBuffer.str();
    const char* fragmentCode = fragmentCodeStr.c_str();
    fFile.close();

    // 3. Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, nullptr);
    glCompileShader(vertexShader);
    GLint success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation error:\n" << infoLog << "\n";
    }

    // 4. Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation error:\n" << infoLog << "\n";
    }

    // 5. Link shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking error:\n" << infoLog << "\n";
    }

    // 6. Clean up shaders (linked into program)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}





int main() {
    /*Define button pixel math
    The proportions are based on the first time I did this project.*/
    float button_width_frac = 5.0f / 24.0f;
    float button_height_frac = 1.0f / 8.0f;
    float x_start_frac = 1.0f / 12.0f;
    float x_spacing_frac = 5.0f / 24.0f;
    float y_frac = 2.0f / 3.0f;
    // Define each button in the array
    vector<Button> buttons;
    float x,y,w,h;
    int k;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 2; j++){
            x = WIN_WIDTH * (x_start_frac + i * x_spacing_frac);
            y = WIN_HEIGHT * (y_frac + ((3.0f/24.0f)*j));
            w =  WIN_WIDTH * button_width_frac;
            h = WIN_HEIGHT * button_height_frac;
            k = j * 4 + i;
            buttons.push_back(Button(x,y,w,h,k));
        }
    }



    glfwSetErrorCallback([](int error, const char* desc) {
        std::cerr << "GLFW Error " << error << ": " << desc << std::endl;
    });

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Configure GLFW (OpenGL version 3.3 core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Basic OpenGL Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    GLuint buttonShaderProgram = loadShaderProgram("shaders/button.vert", "shaders/button.frag");

    // Define units for orthomatrix
    float left = 0.0f;
    float right = WIN_WIDTH;
    float top =  0.0f;
    float bottom = WIN_HEIGHT;
    float near = -1.0f;
    float far = 1.0f;
    // Define Ortho matrix
    float ortho[16] = {
        2.0f/(right-left), 0.0f,              0.0f,               0.0f,
        0.0f,              2.0f/(top-bottom),0.0f,               0.0f,
        0.0f,              0.0f,             -2.0f/(far-near),   0.0f,
       -(right+left)/(right-left), -(top+bottom)/(top-bottom), -(far+near)/(far-near), 1.0f
    };


    // Send ortho to the shader
    GLuint uProjLoc = glGetUniformLocation(buttonShaderProgram, "u_projection");
    glUseProgram(buttonShaderProgram);
    glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, ortho);

    float buttonVertices[] = { // Vertices for buttons
        0.0f, 0.0f,  // bottom-left
        1.0f, 0.0f,  // bottom-right
        1.0f, 1.0f,  // top-right
        0.0f, 0.0f,  // bottom-left
        1.0f, 1.0f,  // top-right
        0.0f, 1.0f   // top-left
    };

    // Set up button VBO and VAO
    GLuint buttonVBO, buttonVAO;
    glGenVertexArrays(1, &buttonVAO);
    glGenBuffers(1, &buttonVBO);
    
    glBindVertexArray(buttonVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, buttonVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(buttonVertices), buttonVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    State currentState = State::D6;

    // Render loop
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        
        glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the button shader program
        glUseProgram(buttonShaderProgram);
        
        // Get uniform locations once (can also do this outside the loop)
        GLuint uPosLoc   = glGetUniformLocation(buttonShaderProgram, "u_position");
        GLuint uSizeLoc  = glGetUniformLocation(buttonShaderProgram, "u_size");
        GLuint uColorLoc = glGetUniformLocation(buttonShaderProgram, "u_color");
        GLuint uProjLoc  = glGetUniformLocation(buttonShaderProgram, "u_projection");
        
        // Set the projection uniform (same for all buttons)
        glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, ortho);
        
        // Bind VAO for the quad
        glBindVertexArray(buttonVAO);

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        bool mousePressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        
        for (Button &button : buttons) {
            // Update button hover
            button.set_selected(button.contains(mouseX, mouseY));

            if (mousePressed && button.contains(mouseX, mouseY)) {
                currentState = static_cast<State>(button.get_index());
                // Debugging line to know what state we are in
                // cout << "currentState = " << stateToString(currentState) << endl;
            }
        
            // Set color uniform
            if (button.is_selected()) { // Darker if hovering
                glUniform3f(uColorLoc, 0.5f, 0.5f, 0.5f);

            } else { // Ligher if not
                glUniform3f(uColorLoc, 0.35f, 0.35f, 0.35f);
            }
        
            // Set position and size uniforms
            glUniform2f(uPosLoc, button.get_x(), button.get_y());
            glUniform2f(uSizeLoc, button.get_width(), button.get_height());
        
            // Draw the quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }





        glfwSwapBuffers(window);
        
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
