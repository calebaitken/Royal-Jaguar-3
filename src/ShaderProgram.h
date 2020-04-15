//
// Created by caleb on 2020-03-11.
//

#ifndef ROYAL_JAGUAR_SHADERPROGRAM_H
#define ROYAL_JAGUAR_SHADERPROGRAM_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"

class Shader;

class Shader {
public:
    Shader() { };

    Shader(std::string vert, std::string frag) {
        int vertShader, fragShader, success;
        GLchar* vertShaderSource, *fragShaderSource;
        unsigned long vertShaderSourceLen, fragShaderSourceLen;
        char infoLog[512];

        // Build & compile shaders
        vertShader = glCreateShader(GL_VERTEX_SHADER);
        fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        if (vertShader == 0 || fragShader == 0) {
            throw std::runtime_error("Error creating shaders");
        }

        if (fileToString(vert, &vertShaderSource, &vertShaderSourceLen) != 0) {
            std::cerr << "Error loading shader file" << std::endl;
            throw std::runtime_error("Failed shader load");
        }
        if (fileToString(frag, &fragShaderSource, &fragShaderSourceLen) != 0) {
            std::cerr << "Error loading shader file" << std::endl;
            throw std::runtime_error("Failed shader load");
        }

        glShaderSource(vertShader, 1, &vertShaderSource, NULL);
        glShaderSource(fragShader, 1, &fragShaderSource, NULL);

        glCompileShader(vertShader);
        glCompileShader(fragShader);

        glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
            throw std::runtime_error("Failed shader compile");
        }

        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
            throw std::runtime_error("Failed shader compile");
        }

        // link shaders
        program = glCreateProgram();
        glAttachShader(program, vertShader);
        glAttachShader(program, fragShader);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
            throw std::runtime_error("Failed shader link");
        }
        glDeleteShader(program);
        glDeleteShader(program);
    };

    Shader use() {
        glUseProgram(this->program);
        return *this;
    }

    int getProgram() {
        return this->program;
    }

    void setFloat(const GLchar* name, GLfloat value) {
        glUniform1f(glGetUniformLocation(this->program, name), value);
    }

    void setInteger(const GLchar* name, GLint value) {
        glUniform1i(glGetUniformLocation(this->program, name), value);
    }

    void setVector2f(const GLchar* name, GLfloat x, GLfloat y) {
        glUniform2f(glGetUniformLocation(this->program, name), x, y);
    }

    void setVector3f(const GLchar* name, glm::vec3& value) {
        glUniform3f(glGetUniformLocation(this->program, name), value.x, value.y, value.z);
    }

    void setMatrix4(const GLchar* name, const glm::mat4& matrix) {
        glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }

private:
    int program = 0;

    int fileToString(const std::string& sourceFile, GLchar** source, unsigned long* len) {
        std::ifstream file;
        file.open(sourceFile, std::ios::in);
        if (!file) {
            return -1;
        }

        file.tellg();
        file.seekg(0, std::ios::end);
        *len = file.tellg();
        file.seekg(std::ios::beg);
        if (*len == 0) {
            return -2;
        }

        *source = (GLchar*) new char[(*len)+1];
        if (*source == nullptr) {
            return -3;
        }

        (*source)[(*len)] = 0;

        for (unsigned int i = 0; file.good(); i++) {
            (*source)[i] = file.get();
            if (file.eof()) {
                (*source)[i] = 0;
                break;
            }
        }

        file.close();

        return 0;
    }
};

#endif //ROYAL_JAGUAR_SHADERPROGRAM_H
