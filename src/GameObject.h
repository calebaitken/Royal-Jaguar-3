//
// Created by caleb on 14/04/2020.
//

#ifndef ROYAL_JAGUAR_3_GAMEOBJECT_H
#define ROYAL_JAGUAR_3_GAMEOBJECT_H

#include <list>

#include "nlohmann/json.hpp"

#include "SpriteRender.h"
#include "TextRender.h"

using json = nlohmann::json;

class GameObject {
public:
    explicit GameObject() = default;
    virtual ~GameObject() = default;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(GameObject&) = delete;

    GameObject(GameObject&&) = default;
    GameObject& operator=(GameObject&&) = default;

    virtual void draw(glm::mat4 projection);
    virtual std::list<std::string> update() = 0;

protected:
    std::unique_ptr<SpriteRender> sprite;
};

class StaticImage : public GameObject {
public:
    explicit StaticImage(const std::string& imageFile);
    explicit StaticImage(const json& j);
    ~StaticImage() override = default;

    StaticImage(const StaticImage&) = delete;
    StaticImage& operator=(StaticImage&) = delete;

    StaticImage(StaticImage&&) = default;
    StaticImage& operator=(StaticImage&&) = default;

    std::list<std::string> update() override {
        return {};
    };
};

class Cursor : public GameObject {
public:
    explicit Cursor(const std::string& imageFile);
    explicit Cursor(const json& j);
    ~Cursor() override = default;

    Cursor(const Cursor&) = delete;
    Cursor& operator=(Cursor&) = delete;

    Cursor(Cursor&&) = default;
    Cursor& operator=(Cursor&&) = default;

    void draw(glm::mat4 projection) override;
    std::list<std::string> update() override;

protected:
    std::unique_ptr<SpriteRender> altSprite;
    bool drawAlt = false;
};

class ImageButton : public StaticImage {
public:
    explicit ImageButton(const json& j);
    ~ImageButton() override = default;

    ImageButton(const ImageButton&) = delete;
    ImageButton& operator=(ImageButton&) = delete;

    ImageButton(ImageButton&&) = default;
    ImageButton& operator=(ImageButton&&) = default;

    std::list<std::string> update() override;

private:
    std::list<std::string> function;
    bool waiting_release = false;
};

class Text : public GameObject {
public:
    explicit Text(const std::shared_ptr<Font>& font, const std::string& string);
    explicit Text(const std::shared_ptr<Font>& font, const json& j);
    explicit Text(const std::shared_ptr<Font>& font, const std::string& string, const json& j);
    ~Text() override = default;

    Text(const Text&) = delete;
    Text& operator=(Text&) = delete;

    Text(Text&&) = default;
    Text& operator=(Text&&) = default;

    void draw(glm::mat4 projection) override;
    std::list<std::string> update() override;

protected:
    std::unique_ptr<TextRender> text;
};

class TextButton : public Text {
public:
    explicit TextButton(const std::shared_ptr<Font>& font, const json& j);
    ~TextButton() override = default;

    TextButton(const TextButton&) = delete;
    TextButton& operator=(TextButton&) = delete;

    TextButton(TextButton&&) = default;
    TextButton& operator=(TextButton&&) = default;

    std::list<std::string> update() override;

private:
    std::list<std::string> function;
    bool waiting_release = false;
};

#endif //ROYAL_JAGUAR_3_GAMEOBJECT_H
