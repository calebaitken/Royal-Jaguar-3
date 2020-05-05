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

    this->base = glGenLists(96);    // store 96 characters

    this->font = CreateFont(-24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, this->fontName.c_str());

    oldFont = (HFONT)SelectObject(this->hDC, this->font);
    wglUseFontBitmaps(this->hDC, 32, 96, this->base);
    SelectObject(this->hDC, oldFont);
    DeleteObject(this->font);
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

    this->data.clear();
    this->data.assign(&buffer[0], &buffer[strlen(buffer)]);
    std::cout << &this->data[0] << std::endl;
}

void TextRender::draw() {
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -1.0f);
    //glColor3f(this->color[0], this->color[1], this->color[2]);
    glColor3f(1.0f*float(cos(cnt1)),1.0f*float(sin(cnt2)),1.0f-0.5f*float(cos(cnt1+cnt2)));
    //glRasterPos2f(this->pos[0], this->pos[1]);
    glRasterPos2f(-0.45f+0.05f*float(cos(cnt1)), 0.35f*float(sin(cnt2)));

    glPushAttrib(GL_LIST_BIT);
    glListBase(this->base - 32);

    glCallLists(this->data.size(), GL_UNSIGNED_SHORT, this->data.data());
    glPopAttrib();

    cnt1+=0.051f;
    cnt2+=0.005f;
}