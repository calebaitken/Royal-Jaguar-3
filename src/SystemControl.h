//
// Created by caleb on 12/04/2020.
//

#ifndef ROYAL_JAGUAR_3_SYSTEMCONTROL_H
#define ROYAL_JAGUAR_3_SYSTEMCONTROL_H

#include <iostream>
#include <list>
#include <vector>
#include <chrono>
#include "nlohmann/json.hpp"
#include "JsonTools/JsonTools.h"

#include "SpriteRender.h"
#include "Window.h"
#include "GameObject.h"
#include "Network.h"

#define MAIN_MENU_JSON "src/resources/data/Index.json"

#define TYPE_MENU "_menu"
#define TYPE_PLAYER "_player"
#define TYPE_STATIC_IMAGE "_staticImage"
#define TYPE_CURSOR "_cursor"
#define TYPE_BUTTON "_button"

#define FLAG_HOST_MENU "_host_menu"
#define FLAG_JOIN_MENU "_join_menu"

using json = nlohmann::json;

class GameLoop;
class Scene;

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    void reset();

    std::list<std::string> update_all();
    void draw_all(glm::mat4 projection);
    template <typename T> void add_object(std::unique_ptr<T> object);
    template <typename T> void add_object(std::unique_ptr<T> object, bool first);
    std::vector<std::unique_ptr<GameObject>> get_objects();

    bool _host_menu = false;
    bool _join_menu = false;

private:
    std::vector<std::unique_ptr<GameObject>> objects;
};

class GameLoop {
public:
    GameLoop() = default;
    ~GameLoop() = default;
    void init();
    void run();

    void load_scene(const std::string& jsonFile);

private:
    Scene scene;
    Window window;
    Network network;
};

#endif //ROYAL_JAGUAR_3_SYSTEMCONTROL_H
