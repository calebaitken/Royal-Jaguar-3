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
#include "TextRender.h"
#include "ResourceManager.h"

#define MAIN_MENU_JSON "src/resources/data/Index.json"

#define TYPE_MENU "_menu"
#define TYPE_PLAYER "_player"
#define TYPE_STATIC_IMAGE "_staticImage"
#define TYPE_CURSOR "_cursor"
#define TYPE_IMAGE_BUTTON "_imageButton"
#define TYPE_TEXT "_text"
#define TYPE_TEXT_BUTTON "_textButton"

#define FLAG_HOST_MENU "_host_menu"
#define FLAG_JOIN_MENU "_join_menu"

using json = nlohmann::json;

class GameLoop;

class GameLoop {
public:
    GameLoop() = default;
    ~GameLoop() = default;
    void init();
    void run();

    void load_scene(const std::string& jsonFile);

private:
    Window window;
    Network network;

    std::map<std::string, std::shared_ptr<Font>> fonts;

    bool _host_menu = false;
    bool _join_menu = false;
};

#endif //ROYAL_JAGUAR_3_SYSTEMCONTROL_H
