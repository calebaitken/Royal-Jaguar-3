//
// Created by caleb on 5/05/2020.
//

#include <functional>
#include "TextRender.h"

Font::Font(const std::string &fontName) {
    this->fontName = fontName;
    this->generate_glyphs();
}

Font::~Font() {
    while (!glyphs.empty()) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &(*this->glyphs.begin()).second.ID);
        std::cout << "Deleted OpenGL glyph: " << (*this->glyphs.begin()).second.ID << std::endl;
        this->glyphs.erase(this->glyphs.begin());
    }
}

void Font::generate_glyphs() {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Could not init freetype library" << std::endl;
    }

    FT_Face face;
    if (FT_New_Face(ft, this->fontName.c_str(), 0, &face)) {
        std::cerr << "Failed to created FT_Face / load font" << std::endl;
    }

    FT_Set_Pixel_Sizes(face, 0, 100);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char character = 0; character < 128; character++) {
        if (FT_Load_Char(face, character, FT_LOAD_RENDER)) {
            std::cerr << "Failed to load glyph: " << character << std::endl;
            continue;
        }

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        std::cout << "Created OpenGL glyph: " << texture << std::endl;
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        // set tecture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Glyph newGlyph = {texture,
                          glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                          glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                          face->glyph->advance.x};
        this->glyphs.insert(std::make_pair(character, newGlyph));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

Glyph Font::get_glyph(const char& key) {
    return this->glyphs.at(key);
}

/*
int FontList::add_font(Font &font) {
    this->fontList.emplace_back(font);
    return this->fontList.size() - 1;
}

void FontList::delete_font(int fontID) {
    this->fontList.erase(this->fontList.begin() + fontID);
}

std::reference_wrapper<Font> FontList::get_font(int fontID) {
    return this->fontList.at(fontID);
}
*/

TextRender::TextRender(const std::shared_ptr<Font>& font, const std::string& text) {
    this->program.reset(new Shader("src/resources/shaders/glyph.vert", "src/resources/shaders/glyph.frag"));
    this->font = font;
    this->text = text;

    glGenVertexArrays(1, &this->VAO);
    std::cout << "Created OpenGL vertex array: " << this->VAO << std::endl;
    glGenBuffers(1, &this->VBO);
    std::cout << "Created OpenGL buffer: " << this->VBO << std::endl;
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRender::~TextRender() {
    this->font.reset();
    this->program.reset(nullptr);
    this->size = glm::vec2(0.0f, 0.0f);
    glDeleteBuffers(1, &this->VBO);
    std::cout << "Deleted OpenGL buffer: " << this->VBO << std::endl;
    glDeleteVertexArrays(1, &this->VAO);
    std::cout << "Deleted OpenGL vertex array: " << this->VAO << std::endl;
}

void TextRender::set_text(const char* format, ...) {
    if (format == NULL) {
        return;
    }

    char buffer[1024];
    va_list ap;

    va_start(ap, format);
        std::vsprintf(buffer, format, ap);
    va_end(ap);

    this->text.assign(buffer);
}

void TextRender::set_font(const std::shared_ptr<Font>& newFont) {
    this->font = newFont;
}

void TextRender::draw(glm::mat4 projection) {
    // set gl render state
    this->program->use();
    this->program->setVector3f("textColor", this->colour);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->VAO);

    glm::vec2 resetValue = this->position;

    // find word length
    float xlen = 0, ylen = 0, yratio, xratio;
    for (auto& charater : this->text) {
        Glyph glyph = this->font->get_glyph(charater);
        xlen += (glyph.advance >> 6);
        if (glyph.size.y > ylen) {
            ylen = glyph.size.y;
        }
    }

    if (this->size.x == 0) {
        this->size.x = xlen;
    }

    xratio = this->size.x / xlen;

    if (this->size.y == 0) {
        this->size.y = ylen * xratio;
    }

    yratio = this->size.y / ylen;


    // iterate through text
    for (auto& character : this->text) {
        Glyph glyph = this->font->get_glyph(character);

        this->program->setInteger("text", 0);
        this->program->setMatrix4("projection", projection);

        // TODO: wacky math here
        float xpos = this->position.x + (glyph.bearing.x * xratio);
        float ypos = this->position.y + (glyph.size.y - glyph.bearing.y) * yratio;
        float w = glyph.size.x * xratio;
        float h = -(glyph.size.y * yratio);

        float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
        };

        glBindTexture(GL_TEXTURE_2D, glyph.ID);

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        this->position.x += (glyph.advance >> 6) * xratio;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    this->position = resetValue;
}