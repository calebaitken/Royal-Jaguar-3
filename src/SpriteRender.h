//
// Created by caleb on 2020-03-11.
//

#ifndef ROYAL_JAGUAR_SPRITERENDER_H
#define ROYAL_JAGUAR_SPRITERENDER_H

#include <cmath>
#include <memory>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"

#include "ShaderProgram.h"
#include "BaseRender.h"

#define VERT_SHADER_FILE "src/resources/shaders/sprite.vert"
#define FRAG_SHADER_FILE "src/resources/shaders/sprite.frag"
#define DEFAULT_IMAGE "src/resources/images/ash_uvgrid01.png"

class Texture2D;
class SpriteRender;

class Texture2D {
public:
    explicit Texture2D() = default;
    explicit Texture2D(const GLchar* file, GLboolean alpha);
    ~Texture2D();

    // non-copyable
    Texture2D(const Texture2D& other) = delete;
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

class SpriteRender : public BaseRender {
public:
    explicit SpriteRender() = default;
    explicit SpriteRender(const std::string& imageFile);
    ~SpriteRender() = default;

    // non-copyable
    SpriteRender(const SpriteRender&) = delete;
    SpriteRender& operator=(SpriteRender&) = delete;

    // movable
    SpriteRender(SpriteRender&&) = default;
    SpriteRender& operator=(SpriteRender&&) = default;

    void draw(glm::mat4 projection) override;

    void scale_to_width();

private:
    std::unique_ptr<Shader> shaderProgram;
    std::unique_ptr<Texture2D> texture;
    unsigned int VAO = 0;
};
#endif //ROYAL_JAGUAR_SPRITERENDER_H
