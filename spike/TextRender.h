//
// Created by caleb on 5/05/2020.
//

#ifndef ROYAL_JAGUAR_3_TEXTRENDER_H
#define ROYAL_JAGUAR_3_TEXTRENDER_H

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <vector>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "glm/glm.hpp"

class TextRender;

class TextRender {
public:
    explicit TextRender(const std::string& fontName);
    ~TextRender();

    // non-copyable
    TextRender(const TextRender&) = delete;
    TextRender& operator=(TextRender&) = delete;

    // movable
    TextRender(TextRender&&) = default;
    TextRender& operator=(TextRender&&) = default;

    void set_string(const char* format, ...);

    void draw();

private:
    void generate_font();
    void destroy_font();

    HDC hDC = NULL;
    std::string fontName;
    HFONT font;
    std::string data;
    GLuint base;

    glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec2 pos = glm::vec2(1.0f, 1.0f);

    float cnt1, cnt2;
};


#endif //ROYAL_JAGUAR_3_TEXTRENDER_H
