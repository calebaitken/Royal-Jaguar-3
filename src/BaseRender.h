//
// Created by caleb on 6/05/2020.
//

#ifndef ROYAL_JAGUAR_3_BASERENDER_H
#define ROYAL_JAGUAR_3_BASERENDER_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class BaseRender;

class BaseRender {
public:
    explicit BaseRender() = default;
    ~BaseRender() = default;

    // non-copyable
    BaseRender(const BaseRender&) = delete;
    BaseRender& operator=(BaseRender&) = delete;

    // movable
    BaseRender(BaseRender&&) = default;
    BaseRender& operator=(BaseRender&&) = default;

    virtual void draw(glm::mat4 projection) = 0;

    glm::vec2 getPosition();
    glm::vec2 getSize();
    glm::vec3 getColour();
    GLfloat getRotation();

    void setPosition(const glm::vec2& newPosition);
    void setPosition(const GLfloat& x, const GLfloat& y);
    void setPosition(const GLfloat& x, const GLfloat& y, bool ratio);
    void setSize(const glm::vec2& newSize);
    void setSize(const GLfloat& x, const GLfloat& y);
    void setSize(const GLfloat& x, const GLfloat& y, bool ratio);
    void setColour(const glm::vec3& newColour);
    void setColour(const GLfloat& r, const GLfloat& g, const GLfloat& b);
    void setRotation(const GLfloat& degrees);


protected:
    glm::vec2 position = glm::vec2(0.0f, 0.0f);
    glm::vec2 size = glm::vec2(1.0f, 1.0f);
    GLfloat rotate = 0.0f;
    glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);
};


#endif //ROYAL_JAGUAR_3_BASERENDER_H
