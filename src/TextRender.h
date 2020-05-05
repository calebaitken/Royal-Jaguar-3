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

class TextRender {
public:
    TextRender(Font& font, const std::string text);
    ~TextRender() = default;

    // non-copyable
    TextRender(const TextRender&) = delete;
    TextRender& operator=(TextRender&) = delete;

    // movable
    TextRender(TextRender&&) = default;
    TextRender& operator=(TextRender&&) = default;

    void set_text(const char* format, ...);
    void set_font(Font& font);

    void draw(glm::mat4 projection);

private:
    unsigned int VAO, VBO; // TODO: do VAO and VBO need to be free'd?
    std::string text;

    glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec2 position = glm::vec2(100.0f, 100.0f);
    float scale = 1.0f;

    Shader program = Shader("src/resources/shaders/glyph.vert", "src/resources/shaders/glyph.frag");
    std::unique_ptr<Font> font;
};


#endif //ROYAL_JAGUAR_3_TEXTRENDER_H
