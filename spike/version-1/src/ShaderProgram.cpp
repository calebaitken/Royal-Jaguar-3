//
// Created by caleb on 5/05/2020.
//

#include "ShaderProgram.h"

Shader::Shader(std::string vert, std::string frag) {
    int vertShader, fragShader, success;
    GLchar* vertShaderSource, * fragShaderSource;
    unsigned long vertShaderSourceLen, fragShaderSourceLen;
    char infoLog[512];

    // Build & compile shaders
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    std::cout << "Created OpenGL shader: " << vertShader << " from: " << vert << std::endl;
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::cout << "Created OpenGL shader: " << fragShader << " from: " << frag << std::endl;

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
    if (!success) {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error("Failed shader compile");
    }

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAG::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error("Failed shader compile");
    }

    // link shaders
    this->program = glCreateProgram();
    glAttachShader(this->program, vertShader);
    glAttachShader(this->program, fragShader);
    glLinkProgram(this->program);

    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error("Failed shader link");
    }

    std::cout << "Created OpenGL program: " << this->program << " from: " << vert << ", " << frag << std::endl;
    glDetachShader(this->program, vertShader);
    glDetachShader(this->program, fragShader);
    glDeleteShader(vertShader);
    std::cout << "Deleted OpenGL shader: " << vertShader << std::endl;
    glDeleteShader(fragShader);
    std::cout << "Deleted OpenGL shader: " << fragShader << std::endl;
}

Shader::~Shader(){
    glDeleteProgram(this->program);
    std::cout << "Deleted OpenGL program: " << this->program << std::endl;
}

int Shader::fileToString(const std::string& sourceFile, GLchar** source, unsigned long* len) {
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

    *source = (GLchar*) new char[(*len) + 1];
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

int Shader::getProgram() {
    return this->program;
}

void Shader::use() {
    glUseProgram(this->program);
}

void Shader::setFloat(const GLchar* name, GLfloat value) {
    glUniform1f(glGetUniformLocation(this->program, name), value);
}

void Shader::setInteger(const GLchar* name, GLint value) {
    glUniform1i(glGetUniformLocation(this->program, name), value);
}

void Shader::setVector2f(const GLchar* name, GLfloat x, GLfloat y) {
    glUniform2f(glGetUniformLocation(this->program, name), x, y);
}

void Shader::setVector3f(const GLchar* name, glm::vec3& value) {
    glUniform3f(glGetUniformLocation(this->program, name), value.x, value.y, value.z);
}

void Shader::setMatrix4(const GLchar* name, const glm::mat4& matrix) {
    glUniformMatrix4fv(glGetUniformLocation(this->program, name), 1, GL_FALSE, glm::value_ptr(matrix));
}