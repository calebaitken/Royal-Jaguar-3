//
// Created by caleb on 14/04/2020.
//

#ifndef ROYAL_JAGUAR_3_GAMEOBJECT_H
#define ROYAL_JAGUAR_3_GAMEOBJECT_H

#include <list>

#include "nlohmann/json.hpp"

#include "SpriteRender.h"

using json = nlohmann::json;

class GameObject {
public:
    explicit GameObject() = default;
    virtual ~GameObject() = default;

    GameObject(const GameObject&) = delete;
    GameObject& operator=(GameObject&) = delete;

    virtual void draw(glm::mat4 projection);
    virtual std::list<std::string> update() = 0;

protected:
    SpriteRender sprite;
};


class StaticImage : public GameObject {
public:
    explicit StaticImage(const std::string& imageFile);
    explicit StaticImage(json& j);
    ~StaticImage() override = default;

    StaticImage(const StaticImage&) = delete;
    StaticImage& operator=(StaticImage&) = delete;

    std::list<std::string> update() override {
        return {};
    };
};

class Cursor : public GameObject {
public:
    explicit Cursor(const std::string& imageFile);
    explicit Cursor(json& j);
    ~Cursor() override = default;

    Cursor(const Cursor&) = delete;
    Cursor& operator=(Cursor&) = delete;

    void draw(glm::mat4 projection) override;
    std::list<std::string> update() override;

protected:
    SpriteRender altSprite;
    bool drawAlt = false;
};

class Button : public StaticImage {
public:
    explicit Button(json& j);
    ~Button() override = default;

    Button(const Button&) = delete;
    Button& operator=(Button&) = delete;

    std::list<std::string> update() override;

private:
    std::list<std::string> function;
    bool waiting_release = false;
};

#endif //ROYAL_JAGUAR_3_GAMEOBJECT_H
