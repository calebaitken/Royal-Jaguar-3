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
    explicit Shader() = default;
    Shader(std::string vert, std::string frag);
    ~Shader();

    // non-copyable
    Shader(const Shader&) = delete;
    Shader& operator=(Shader&) = delete;

    // movable
    Shader(Shader&&) = default;
    Shader& operator=(Shader&&) = default;

    int getProgram();

    void use();

    void setFloat(const GLchar* name, GLfloat value);
    void setInteger(const GLchar* name, GLint value);
    void setVector2f(const GLchar* name, GLfloat x, GLfloat y);
    void setVector3f(const GLchar* name, glm::vec3& value);
    void setMatrix4(const GLchar* name, const glm::mat4& matrix);

private:
    int program;

    int fileToString(const std::string& sourceFile, GLchar** source, unsigned long* len);
};

#endif //ROYAL_JAGUAR_SHADERPROGRAM_H
