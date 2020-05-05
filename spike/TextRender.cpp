//
// Created by caleb on 5/05/2020.
//

#include "TextRender.h"

TextRender::TextRender(const std::string& fontName) {
    this->fontName = fontName;
    this->hDC = GetDC(glfwGetWin32Window(glfwGetCurrentContext()));
    this->generate_font();
}

TextRender::~TextRender() {
    this->destroy_font();
}

void TextRender::generate_font() {
    HFONT oldFont;

    this->base = glGenLists(127);    // store 96 characters

    this->font = CreateFont(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 0, 0, 0, 0, this->fontName.c_str());

    oldFont = (HFONT) SelectObject(this->hDC, this->font);
    wglUseFontBitmaps(this->hDC, 0, 127, this->base);
    wglUseFontBitmaps(this->hDC, 0, 127, this->base);
    //SelectObject(this->hDC, oldFont);
    //DeleteObject(this->font);
}

void TextRender::destroy_font() {
    glDeleteLists(this->base, 96);
}

void TextRender::set_string(const char* format, ...) {
    if (format == NULL) {
        return;
    }

    char buffer[1024];
    va_list ap;

    va_start(ap, format);
        std::vsprintf(buffer, format, ap);
    va_end(ap);

    this->data.assign(buffer);
}

void TextRender::draw() {
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -1.0f);
    //glColor3f(this->color[0], this->color[1], this->color[2]);
    glColor3f(1.0f, 0.0f, 0.0f);
    //glRasterPos2f(this->pos[0], this->pos[1]);
    glRasterPos3i(-1, 0, 5);

    glPushAttrib(GL_LIST_BIT);
    glListBase(this->base);

    glCallLists(this->data.length(), GL_UNSIGNED_BYTE, this->data.c_str());
    glPopAttrib();


    cnt1+=0.051f;
    cnt2+=0.005f;
}