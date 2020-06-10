//
// Created by caleb on 5/05/2020.
//

#ifndef ROYAL_JAGUAR_3_TEXTRENDER_H
#define ROYAL_JAGUAR_3_TEXTRENDER_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "ft2build.h"
#include FT_FREETYPE_H
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "ShaderProgram.h"
#include "BaseRender.h"

struct Charater;
class Font;
class TextRender;

/**
 * Wrapper for all data need to render a glyph.
 */
struct Glyph {
    unsigned int ID;    // ID handle of texture in OpenGL memory
    glm::ivec2 size;    // Size of glyph
    glm::ivec2 bearing; // Offset from baseline to left/top of glyph
    long advance;       // Offset to advance to next glyph
};

class Font {
public:
    explicit Font() = default;
    explicit Font(const std::string& fontName);
    ~Font();

    // non-copyable
    Font(const Font&) = delete;
    Font& operator=(Font&) = delete;

    // movable
    Font(Font&&) = default;
    Font& operator=(Font&&) = default;

    Glyph get_glyph(const char& key);

private:
    void generate_glyphs();

    std::string fontName;
    std::map<char, Glyph> glyphs;
};

/* REMOVED
class FontList {
public:
    FontList() = default;
    ~FontList() = default;
    FontList(const FontList&) = delete;
    FontList& operator=(FontList&) = delete;
    FontList(FontList&&) = default;
    FontList& operator=(FontList&&) = default;

    int add_font(Font& font);
    void delete_font(int fontID);

    std::reference_wrapper<Font> get_font(int fontID);

private:
    std::vector<std::reference_wrapper<Font>> fontList;
};
*/

class TextRender : public BaseRender {
public:
    explicit TextRender() = default;
    TextRender(const std::shared_ptr<Font>& font, const std::string& text);
    ~TextRender();

    // non-copyable
    TextRender(const TextRender&) = delete;
    TextRender& operator=(TextRender&) = delete;

    // movable
    TextRender(TextRender&&) = default;
    TextRender& operator=(TextRender&&) = default;

    void draw(glm::mat4 projection);

    void set_text(const char* format, ...);
    void set_font(const std::shared_ptr<Font>& newFont);

private:
    unsigned int VAO = 0, VBO = 0;
    std::string text;

    std::unique_ptr<Shader> program;
    std::shared_ptr<Font> font;
};


#endif //ROYAL_JAGUAR_3_TEXTRENDER_H
