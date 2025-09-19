#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>

class Shader {
	private:
		GLuint shaderProgram;
	public:
		Shader(const std::string& vertexPath, const std::string& fragPath);
		
		void use() const;
		
		GLuint getID() const;
	private:
		/*
		GLuint loadShaderProgram
		Purpose:
			- The purpose of loadShaderProgram is to load vertex
			and fragment shaders
			1. Read the vertex shader using ifstream store as c_str()
			2. Read the fragment shader using ifstream store as c_str();
			3. Compile vertex and fragment shader
			4. Link the shader program
				- Attach each shader
				- Link program
				- Error check
			5. Delete shaders (now linked)
		Args: 
		- const std::string& vertexPath
			- Path to vertex shader program
		- const std::string& fragPath
			- Path to fragment shader program
		Return:
			- Returns the GLuint shaderProgram
		Pottential Places for Errors to Raise:
			- Reading each shader file (can not find the shader/failed to open)
			- Compiling each shader (error in glsl)
			- Failure to Link

		*/
		GLuint loadShaderProgram(const std::string& vertexPath, const std::string& fragPath);

};

#endif