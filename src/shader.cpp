#include "shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragPath){
	shaderProgram = loadShaderProgram(vertexPath, fragPath);
}

void Shader::use() const{
	glUseProgram(shaderProgram);
}

GLuint Shader::getID() const{
	return shaderProgram;
}

GLuint Shader::loadShaderProgram(const std::string& vertexPath, const std::string& fragPath){
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
    std::ifstream fFile(fragPath);
    if (!fFile.is_open()) {
        std::cerr << "Failed to open fragment shader: " << fragPath << "\n";
        exit(-1);
    }
    std::stringstream fBuffer;
    fBuffer << fFile.rdbuf();
    std::string fragCodeStr = fBuffer.str();
    const char* fragCode = fragCodeStr.c_str();
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
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragCode, nullptr);
    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation error:\n" << infoLog << "\n";
    }

    // 5. Link shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking error:\n" << infoLog << "\n";
    }

    // 6. Clean up shaders (linked into program)
    glDeleteShader(vertexShader);
    glDeleteShader(fragShader);

    return shaderProgram;
}