//
// Created by caleb on 6/05/2020.
//

#include "BaseRender.h"

glm::vec2 BaseRender::getPosition() {
    return glm::vec2(this->position);
}

glm::vec2 BaseRender::getSize() {
    return glm::vec2(this->size);
}

glm::vec3 BaseRender::getColour() {
    return glm::vec3(this->colour);
}

GLfloat BaseRender::getRotation() {
    return GLfloat(this->rotate);
}

void BaseRender::setPosition(const glm::vec2& newPosition) {
    this->position = newPosition;
}

void BaseRender::setPosition(const GLfloat &x, const GLfloat &y) {
    this->position = glm::vec2(x, y);
}

void BaseRender::setPosition(const GLfloat &x, const GLfloat &y, bool ratio) {
    if (ratio) {
        this->position = glm::vec2(this->position[0] * x, this->position[1] * y);
    } else {
        this->position = glm::vec2(x, y);
    }
}

void BaseRender::setSize(const glm::vec2 &newSize) {
    this->size = newSize;
}

void BaseRender::setSize(const GLfloat &x, const GLfloat &y) {
    this->size = glm::vec2(x, y);
}

void BaseRender::setSize(const GLfloat &x, const GLfloat &y, bool ratio) {
    if (ratio) {
        this->size = glm::vec2(this->size[0] * x, this->size[1] * y);
    } else {
        this->size = glm::vec2(x, y);
    }
}

void BaseRender::setColour(const glm::vec3 &newColour) {
    this->colour = newColour;
}

void BaseRender::setColour(const GLfloat &r, const GLfloat &g, const GLfloat &b) {
    this->colour = glm::vec3(r, g, b);
}

void BaseRender::setRotation(const GLfloat &degrees) {
    this->rotate = GLfloat(degrees);
}