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
    explicit Texture2D(const GLchar* file, GLboolean alpha);
    ~Texture2D();

    // non-copyable
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(Texture2D&) = delete;

    // movable
    Texture2D(Texture2D&&) = default;
    Texture2D& operator=(Texture2D&&) = default;

    GLuint getID();
    GLuint getWidth();
    GLuint getHeight();

private:
    void generate(GLuint width, GLuint height, unsigned char* data);

    GLuint ID;
    GLuint width, height;

    GLuint internalFormat;
    GLuint imageFormat;
};

class SpriteRender {
public:
    explicit SpriteRender(const std::string& imageFile = DEFAULT_IMAGE);
    ~SpriteRender() = default;

    // non-copyable
    SpriteRender(const SpriteRender&) = delete;
    SpriteRender& operator=(SpriteRender&) = delete;

    // movable
    SpriteRender(SpriteRender&&) = default;
    SpriteRender& operator=(SpriteRender&&) = default;

    void draw(glm::mat4 projection);

    glm::vec2 getPosition();
    glm::vec2 getSize();
    glm::vec3 getColour();
    GLfloat getRotation();

    void setTexture(const std::string& imageFile);
    void setPosition(glm::vec2 newPosition);
    void setPosition(GLfloat x, GLfloat y);
    void setPosition(GLfloat x, GLfloat y, bool ratio);
    void setSize(glm::vec2 newSize);
    void setSize(GLfloat x, GLfloat y);
    void setSize(GLfloat x, GLfloat y, bool ratio);
    void setColour(glm::vec3 newColour);
    void setColour(GLfloat r, GLfloat g, GLfloat b);
    void setRotation(GLfloat degrees);

    void scale_to_width();

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
