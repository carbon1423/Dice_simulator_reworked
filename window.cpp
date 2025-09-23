using namespace std;
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <glm/gtc/epsilon.hpp>
#include "dice.h"
#include "button.h"
#include "shader.h"

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

/*
stateToString(State s)
- This is a helper function to allow me to print what state we are in
because I cannot print an enum object
*/
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

bool approxEqual(const glm::mat4& a, const glm::mat4& b, float eps) {
    for (int i = 0; i < 4; ++i) {
        if (!glm::all(glm::epsilonEqual(a[i], b[i], eps))) {
            return false;
        }
    }
    return true;
}


/*renderSelect(State s)
- Take in the current state and figure out which vertexes to render based on the state*/
void renderSelect(State s){
    switch(s){
    case State::D4:  glDrawElements(GL_TRIANGLES, 12,  GL_UNSIGNED_INT, (void*)(0)); break;
    case State::D6:  glDrawElements(GL_TRIANGLES, 36,  GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint))); break;
    case State::D8:  glDrawElements(GL_TRIANGLES, 24,  GL_UNSIGNED_INT, (void*)(48 * sizeof(GLuint))); break;
    case State::D10: glDrawElements(GL_TRIANGLES, 60,  GL_UNSIGNED_INT, (void*)(72 * sizeof(GLuint))); break;
    case State::D12: glDrawElements(GL_TRIANGLES, 108, GL_UNSIGNED_INT, (void*)((72+60) * sizeof(GLuint))); break;
    case State::D20: glDrawElements(GL_TRIANGLES, 60,  GL_UNSIGNED_INT, (void*)((72+60+108) * sizeof(GLuint))); break;
    }

}


/*buildDiceVertices is a helper function to calculate the normals of each face 
and inlcude it in our flattened vertice vector*/
std::vector<Vertex> buildDiceVertices(const std::vector<Dice>& diceVec, std::vector<unsigned int>& outIndices){
    std::vector<Vertex> vertices;
    outIndices.clear();

    unsigned int vertexOffset = 0;

    for (const Dice& die : diceVec) {
        const auto& points = die.get_points();
        const auto& tris   = die.get_triangles();

        for (const auto& tri : tris) {
            glm::vec3 v0 = points[tri.i];
            glm::vec3 v1 = points[tri.j];
            glm::vec3 v2 = points[tri.k];

            // Compute per-face normal
            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            // Add three vertices with the same normal
            vertices.push_back({v0, normal});
            vertices.push_back({v1, normal});
            vertices.push_back({v2, normal});

            // Indices (local to this triangle)
            outIndices.push_back(vertexOffset + 0);
            outIndices.push_back(vertexOffset + 1);
            outIndices.push_back(vertexOffset + 2);

            vertexOffset += 3;
        }
    }

    return vertices;
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

    // Define a vector to hold each dice object
    vector<Dice> diceVec;
    diceVec.push_back(Dice(4));
    diceVec.push_back(Dice(6));
    diceVec.push_back(Dice(8));
    diceVec.push_back(Dice(10));
    diceVec.push_back(Dice(12));
    diceVec.push_back(Dice(20));



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





    Shader buttonShader("shaders/button.vert", "shaders/button.frag");

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
    GLuint uProjLoc = glGetUniformLocation(buttonShader.getID(), "u_projection");
    buttonShader.use();
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


    // Here we will set up the diceVBO, VAO, and EBO
    // This will allow our dice to render
    //Initialize the shader program for the dice
    Shader diceShader("shaders/dice.vert","shaders/dice.frag");

    //Next we flatten the vertex data
    vector<unsigned int> diceIndices;
    vector<Vertex> diceVertices = buildDiceVertices(diceVec, diceIndices);

    // Set up our VBO, EBO, and VAO
    GLuint diceVBO, diceEBO, diceVAO;
    glGenBuffers(1, &diceVBO);
    glGenBuffers(1, &diceEBO);
    glGenVertexArrays(1, &diceVAO);

    glBindVertexArray(diceVAO);

    glBindBuffer(GL_ARRAY_BUFFER, diceVBO);
    glBufferData(GL_ARRAY_BUFFER, diceVertices.size() * sizeof(Vertex), diceVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, diceEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, diceIndices.size() * sizeof(unsigned int), diceIndices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    
    

    glBindVertexArray(0);


    glEnable(GL_DEPTH_TEST);

    diceShader.use();

    glm::mat4 model = glm::mat4(1.0f);

    GLint modelLoc       = glGetUniformLocation(diceShader.getID(), "model");
    GLint viewLoc        = glGetUniformLocation(diceShader.getID(), "view");
    GLint projLoc        = glGetUniformLocation(diceShader.getID(), "projection");
    GLint lightPosLoc    = glGetUniformLocation(diceShader.getID(), "lightPos");
    GLint viewPosLoc     = glGetUniformLocation(diceShader.getID(), "viewPos");
    GLint lightColorLoc  = glGetUniformLocation(diceShader.getID(), "lightColor");
    GLint objectColorLoc = glGetUniformLocation(diceShader.getID(), "objectColor");
    
    //quick debug if a uniform wasn't found (misspelled in shader or optimized out)
    if (modelLoc == -1)       std::cerr << "Warning: 'model' uniform not found\n";
    if (viewLoc == -1)        std::cerr << "Warning: 'view' uniform not found\n";
    if (projLoc == -1)        std::cerr << "Warning: 'projection' uniform not found\n";
    if (lightPosLoc == -1)    std::cerr << "Warning: 'lightPos' uniform not found\n";
    if (viewPosLoc == -1)     std::cerr << "Warning: 'viewPos' uniform not found\n";
    if (lightColorLoc == -1)  std::cerr << "Warning: 'lightColor' uniform not found\n";
    if (objectColorLoc == -1) std::cerr << "Warning: 'objectColor' uniform not found\n";




    State currentState = State::D4;
    State prevState = State::D4;

    float lastFrame = 0.0f;  // time of last frame
    float deltaTime = 0.0f;
    float rotationAngle = 0.0f;

    std::random_device rd;  
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_real_distribution<float> dist(0.0f, 1.0f); 

    bool isRolling = false;

    // glDisable(GL_CULL_FACE);

    // Render loop
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime();      // seconds since init
        deltaTime = (currentFrame - lastFrame);    // difference from last frame
        lastFrame = currentFrame;

        glfwPollEvents();
        
        

        glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the button shader program
        buttonShader.use();
        
        // Get uniform locations once (can also do this outside the loop)
        GLuint uPosLoc   = glGetUniformLocation(buttonShader.getID(), "u_position");
        GLuint uSizeLoc  = glGetUniformLocation(buttonShader.getID(), "u_size");
        GLuint uColorLoc = glGetUniformLocation(buttonShader.getID(), "u_color");
        GLuint uProjLoc  = glGetUniformLocation(buttonShader.getID(), "u_projection");
        
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
                prevState = currentState;
                // cout << "prevState" << stateToString(prevState) << endl;
                if(static_cast<State>(button.get_index()) != State::ROLL){
                    currentState = static_cast<State>(button.get_index());
                }else{
                    isRolling = true;
                    rotationAngle = 0.0f;
                }
                
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

        diceShader.use();

        glViewport(0, 200,WIN_WIDTH,400);
        float portionAspect = WIN_WIDTH/(2 * WIN_HEIGHT/3);
        float rand = dist(gen);

        if(isRolling){
            rotationAngle += deltaTime * 2.0f;
            // cout << "rand" << 10 * rand << endl;
            model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(10 * rand, 10 * rand, 10 *rand));
            // cout << rotationAngle << endl;
            if(rotationAngle >= 2 * M_PI){
                rotationAngle = 0.0f;
                // currentState = prevState;
                isRolling = false;
                // model = glm::mat4(1.0f);
                cout << stateToString(currentState) << endl;
            }
        }else if(!approxEqual(model, glm::mat4(1.0f), 0.001f)){
            // glm::mat4 modelSubI = model - glm::mat4(1.0f);
            // model = model - (0.00001f * modelSubI);
            glm::quat curr = glm::quat_cast(model);
            glm::quat target = glm::quat(1,0,0,0);
            curr = glm::slerp(curr,target,0.01f);

            glm::vec3 position = glm::vec3(model[3]);
            glm::vec3 scale(1.0f);

        
            // rebuild full transform
            glm::mat4 rot = glm::mat4_cast(curr);
            model = glm::translate(glm::mat4(1.0f), position) *
                    rot *
                    glm::scale(glm::mat4(1.0f), scale);
        }else if(approxEqual(model, glm::mat4(1.0f), 0.001f)){
            model = glm::mat4(1.0f);
        }
        
        // simple static camera at (0,0,3) looking at origin
        glm::vec3 camPos   = glm::vec3(0.0f, 0.0f, -3.0f);
        glm::vec3 camTarget= glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 camUp    = glm::vec3(0.0f, 1.0f, 0.0f);
        
        glm::mat4 view = glm::lookAt(camPos, camTarget, camUp);
        
        // fixed perspective projection
        float zoom = 2.0f;

        glm::mat4 projection = glm::ortho(
            -portionAspect * zoom, portionAspect * zoom,  // left, right
            -zoom, zoom,  // bottom, top
            0.1f, 100.0f   // near, far
        );

        // glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.5f,0.1f,100.0f);
        
        // send matrices
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        // --- Lighting ---
        glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -10.0f); // example light position
        glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
        
        // use camPos for view position
        glUniform3fv(viewPosLoc, 1, glm::value_ptr(camPos));
        
        glUniform3f(lightColorLoc, 0.8f, 0.8f, 0.8f);
        glUniform3f(objectColorLoc, 0.8f, 0.3f, 0.3f); // red dice
        
        // --- Draw ---
        glBindVertexArray(diceVAO);
        // glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
        renderSelect(currentState);
        glBindVertexArray(0);
        

        glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);

        glfwSwapBuffers(window);
        
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
