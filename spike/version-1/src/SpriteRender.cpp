//
// Created by caleb on 5/05/2020.
//

#include "SpriteRender.h"

Texture2D::Texture2D(const GLchar* file, GLboolean alpha) : width(0), height(0), internalFormat(GL_RGBA), imageFormat(GL_RGBA) {
    glGenTextures(1, &this->ID);
    std::cout << "Created OpenGL texture: " << this->ID << " from: " << file << std::endl;
    if (alpha) {
        this->internalFormat = GL_RGBA;
        this->imageFormat = GL_RGBA;
    }

    int imageWidth, imageHeight;
    unsigned char* image = SOIL_load_image(file, &imageWidth, &imageHeight, nullptr, this->imageFormat == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    this->generate(imageWidth, imageHeight, image);

    SOIL_free_image_data(image);
}

Texture2D::~Texture2D(){
    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &this->ID);
    std::cout << "Deleted OpenGL texture: " << this->ID << std::endl;
}

void Texture2D::generate(GLuint width, GLuint height, unsigned char* data) {
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

GLuint Texture2D::getID() {
    return this->ID;
}

GLuint Texture2D::getWidth() {
    return this->width;
}

GLuint Texture2D::getHeight() {
    return this->height;
}

SpriteRender::SpriteRender(const std::string& imageFile) {
    this->texture.reset(new Texture2D(imageFile.c_str(), GL_TRUE));
    this->shaderProgram.reset(new Shader(VERT_SHADER_FILE, FRAG_SHADER_FILE));
    this->size = glm::vec2(this->texture->getWidth(), this->texture->getHeight());
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
    std::cout << "Created OpenGL vertex array: " << this->VAO << std::endl;
    glGenBuffers(1, &VBO);
    std::cout << "Created OpenGL buffer: " << VBO << std::endl;

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &VBO);
    std::cout << "Deleted OpenGL buffer: " << VBO << std::endl;

}

SpriteRender::~SpriteRender() {
    this->shaderProgram.reset(nullptr);
    this->texture.reset(nullptr);
    glDeleteVertexArrays(1, &this->VAO);
    std::cout << "Deleted OpenGL vertex array: " << this->VAO << std::endl;
}

void SpriteRender::scale_to_width() {
    int width;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, nullptr);
    this->setSize(width, width * (GLfloat(this->texture->getHeight()) / GLfloat(this->texture->getWidth())));
}

void SpriteRender::draw(glm::mat4 projection) {
    shaderProgram->use();

    this->shaderProgram->setInteger("image", 0);
    this->shaderProgram->setMatrix4("projection", projection);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(this->position, 0.0f));

    model = glm::translate(model, glm::vec3(0.5f * this->size.x, 0.5f * this->size.y, 0.0f));
    model = glm::rotate(model, this->rotate, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * this->size.x, -0.5f * this->size.y, 0.0f));

    model = glm::scale(model, glm::vec3(this->size, 1.0f));

    this->shaderProgram->setMatrix4("model", model);

    this->shaderProgram->setVector3f("spriteColour", this->colour);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture->getID());

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}