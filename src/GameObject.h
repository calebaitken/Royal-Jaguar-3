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
    GameObject() = default;
    ~GameObject() = default;

    virtual void draw(glm::mat4 projection);
    virtual std::list<std::string> update() = 0;

protected:
    SpriteRender sprite;
};


class StaticImage : public GameObject {
public:
    explicit StaticImage(std::string imageFile);
    explicit StaticImage(json& j);

    std::list<std::string> update() override {
        return {};
    };
};

class Cursor : public GameObject {
public:
    explicit Cursor(std::string imageFile);
    explicit Cursor(json& j);

    void draw(glm::mat4 projection) override;
    std::list<std::string> update() override;

protected:
    SpriteRender altSprite;
    bool drawAlt = false;
};

class Button : public StaticImage {
public:
    explicit Button(json& j);
    std::list<std::string> update() override;

private:
    std::list<std::string> function;
    bool waiting_release = false;
};

#endif //ROYAL_JAGUAR_3_GAMEOBJECT_H
