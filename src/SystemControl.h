//
// Created by caleb on 12/04/2020.
//

#ifndef ROYAL_JAGUAR_3_SYSTEMCONTROL_H
#define ROYAL_JAGUAR_3_SYSTEMCONTROL_H

#include <iostream>
#include <list>
#include <vector>
#include "nlohmann/json.hpp"
#include "JsonTools/JsonTools.h"

#include "SpriteRender.h"
#include "Window.h"
#include "GameObject.h"

#define TYPE_MENU "_menu"
#define TYPE_PLAYER "_player"
#define TYPE_STATIC_IMAGE "_staticImage"
#define TYPE_CURSOR "_cursor"
#define TYPE_BUTTON "_button"

using json = nlohmann::json;

class GameLoop;
class Scene;

class Scene {
public:
    Scene() = default;
    ~Scene() = default;
    std::list<std::string> update_all();
    void draw_all(glm::mat4 projection);
    template <typename T> void add_object(T object);
    template <typename T> void add_object(T object, bool first);
    std::vector<GameObject*> get_objects();

private:
    std::vector<GameObject*> objects;
};

class GameLoop {
public:
    GameLoop() = default;
    ~GameLoop() = default;
    void init();
    void run();

    void load_scene(std::string jsonFile);

private:
    Scene scene;
    Window window;
};

namespace SystemControl {
    static void reload_scene(GameLoop* game, std::string jsonFile) {
        game->load_scene(jsonFile);
    }
}

#endif //ROYAL_JAGUAR_3_SYSTEMCONTROL_H
