#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader{
public:
	//program ID
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath){
		std::string vertexSource, fragmentSource;
		std::ifstream vertexFile, fragmentFile;
		//exceptions
		vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try{
			//open our files
			vertexFile.open(vertexPath);
			fragmentFile.open(fragmentPath);
			//create a stream of strings
			std::stringstream vertexStream, fragmentStream;
			//istream file buffer contents into sstream
			vertexStream << vertexFile.rdbuf();
			fragmentStream << fragmentFile.rdbuf();
			//close file after we're done
			vertexFile.close();
			fragmentFile.close();
			//
			vertexSource = vertexStream.str();
			fragmentSource = fragmentStream.str();
		}
		catch(std::ifstream::failure e)
		{
			std::cerr << "ERROR::READING::SHADER::FILES" << std::endl;
		}
		const char* vertexShaderCode = vertexSource.c_str();
		const char* fragmentShaderCode = fragmentSource.c_str();

		//compile shaders
		unsigned int vertex, fragment;
		int success;
		char infolog[512];
		//compile vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(vertex, 512, NULL ,infolog);
			std::cerr << "FAILED::TO::COMPILE::VERTEX::SHADER\n" << infolog << std::endl;
		}
		//compile fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetShaderInfoLog(fragment, 512, NULL, infolog);
			std::cerr << "FAILED::TO::COMPILE::FRAGMENT::SHADER\n" << infolog << std::endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_COMPILE_STATUS, &success);
		if(!success){
			glGetProgramInfoLog(ID, 512, NULL, infolog);
			std::cerr << "FAILED:TO::LINK::Shader::PROGRAM\n" << infolog << std::endl;
		}
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	
	//use the shader program
	void use(){
		glUseProgram(ID);
	}
	void setBool(const std::string &name,bool value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

};

#endif
