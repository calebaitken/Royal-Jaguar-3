//
// Created by caleb on 2020-03-11.
//

#ifndef ROYAL_JAGUAR_SPRITERENDER_H
#define ROYAL_JAGUAR_SPRITERENDER_H

#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"

#include "ShaderProgram.h"

#define VERT_SHADER_FILE "src/resources/shaders/sprite.vert"
#define FRAG_SHADER_FILE "src/resources/shaders/sprite.frag"
#define DEFAULT_IMAGE "src/resources/images/ash_uvgrid01.png"

class Texture2D;
class SpriteRender;

class Texture2D {
public:
    ~Texture2D() {
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &this->ID);
        std::cout << "texture " << this->ID << " deleted" << std::endl;
    }

    Texture2D(const GLchar* file, GLboolean alpha) : width(0), height(0), internalFormat(GL_RGBA), imageFormat(GL_RGBA) {
        glGenTextures(1, &this->ID);
        std::cout << "texture " << this->ID << " made from: " << file << std::endl;
        if (alpha) {
            internalFormat = GL_RGBA;
            imageFormat = GL_RGBA;
        }

        int imageWidth, imageHeight;
        unsigned char* image = SOIL_load_image(file, &imageWidth, &imageHeight, nullptr, imageFormat == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
        this->generate(imageWidth, imageHeight, image);
        SOIL_free_image_data(image);
    }

    GLuint getID() {
        return ID;
    }

    GLuint getWidth() {
        return this->width;
    }

    GLuint getHeight() {
        return this->height;
    }

private:
    void generate(GLuint width, GLuint height, unsigned char* data) {
        this->width = width;
        this->height = height;

        glBindTexture(GL_TEXTURE_2D, this->ID);
        glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, width, height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint ID;
    GLuint width, height;

    GLuint internalFormat;
    GLuint imageFormat;
};

class SpriteRender {
public:
    ~SpriteRender() = default;

    explicit SpriteRender(const std::string& imageFile = DEFAULT_IMAGE) : texture(imageFile.c_str(), GL_TRUE), shaderProgram(VERT_SHADER_FILE, FRAG_SHADER_FILE) {
        this->rotate = 0.0f;
        this->size = glm::vec2(this->texture.getWidth(), this->texture.getHeight());
        unsigned int VBO;

        float vertices[] = {
                // Pos      // Tex
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,

                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f
        };

        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(this->VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->VAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    void draw(glm::mat4 projection) {
        glUseProgram(shaderProgram.getProgram());

        this->shaderProgram.setInteger("image", 0);
        this->shaderProgram.setMatrix4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(this->position, 0.0f));

        model = glm::translate(model, glm::vec3(0.5f * this->size.x, 0.5f * this->size.y, 0.0f));
        model = glm::rotate(model, this->rotate, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * this->size.x, -0.5f * this->size.y, 0.0f));

        model = glm::scale(model, glm::vec3(this->size, 1.0f));

        this->shaderProgram.setMatrix4("model", model);

        this->shaderProgram.setVector3f("spriteColour", this->colour);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texture.getID());

        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    void setTexture(const std::string& imageFile) {
        this->texture = Texture2D(imageFile.c_str(), GL_TRUE);
    }

    void setPosition(glm::vec2 newPosition) {
        this->position = newPosition;
    }

    void setPosition(GLfloat x, GLfloat y) {
        this->position = glm::vec2(x, y);
    }

    void setPosition(GLfloat x, GLfloat y, bool ratio) {
        if (ratio) {
            this->position = glm::vec2(this->position[0] * x, this->position[1] * y);
        } else {
            this->position = glm::vec2(x, y);
        }
    }

    glm::vec2 getPosition() {
        return glm::vec2(this->position);
    }

    void setSize(glm::vec2 newSize) {
        this->size = newSize;
    }

    void setSize(GLfloat x, GLfloat y) {
        this->size = glm::vec2(x, y);
    }

    void setSize(GLfloat x, GLfloat y, bool ratio) {
        if (ratio) {
            this->size = glm::vec2(this->size[0] * x, this->size[1] * y);
        } else {
            this->size = glm::vec2(x, y);
        }
    }

    glm::vec2 getSize() {
        return glm::vec2(this->size);
    }

    void setColour(glm::vec3 newColour) {
        this->colour = newColour;
    }

    void setColour(GLfloat r, GLfloat g, GLfloat b) {
        this->colour = glm::vec3(r, b, g);
    }

    glm::vec3 getColour() {
        return glm::vec3(this->colour);
    }

    void setRotation(GLfloat degrees) {
        this->rotate = degrees;
    }

    GLfloat getRotation() {
        return GLfloat(this->rotate);
    }

    void scale_to_width() {
        int width;
        glfwGetWindowSize(glfwGetCurrentContext(), &width, nullptr);
        this->setSize(width, width * (GLfloat(this->texture.getHeight()) / GLfloat(this->texture.getWidth())));
    }

private:
    Shader shaderProgram;
    Texture2D texture;
    glm::vec2 position;
    glm::vec2 size;
    GLfloat rotate;
    glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);
    unsigned int VAO;
};
#endif //ROYAL_JAGUAR_SPRITERENDER_H
