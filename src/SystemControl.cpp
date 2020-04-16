//
// Created by caleb on 12/04/2020.
//

#include "SystemControl.h"

#define MAIN_MENU_JSON "src/resources/data/Index.json"

std::vector<GameObject*> Scene::get_objects() {
    return this->objects;
}

template <typename T>
void Scene::add_object(T object) {
    static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
    this->objects.insert(this->objects.begin(), new T(object));}

template <typename T>
void Scene::add_object(T object, bool first) {
    static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
    if (!first) {
        this->objects.insert(this->objects.begin(), new T(object));
    } else {
        this->objects.emplace_back(new T(object));
    }
}

std::list<std::string> Scene::update_all() {
    std::list<std::string> result, value;
    for (int i = 0; i < objects.size(); i++) {
        result.splice(result.end(), objects[i]->update());
    }

    return result;
}

void Scene::draw_all(glm::mat4 projection) {
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->draw(projection);
    }
}

void GameLoop::init() {
    this->window = Window(0, 0);
    this->load_scene(MAIN_MENU_JSON);
}

void GameLoop::run() {
    std::list<std::string> gameObjectReturns;
    while(this->window.get_state()) {
        // get user input
        glfwWaitEvents();

        // update game state
        gameObjectReturns = this->scene.update_all();
        for (auto iter = gameObjectReturns.begin(); iter != gameObjectReturns.end(); iter++) {
            if ((*iter) == "_load_scene") {
                iter++;
                this->load_scene(*iter);
                break;
            } else if ((*iter) == "_quit_game") {
                this->window.terminate();
                break;
            }
        }

        // draw frame
        this->window.clear_buffer();
        this->scene.draw_all(this->window.get_projection_mat());
        this->window.swap_buffer();
    }
}

void GameLoop::load_scene(std::string jsonFile) {
    json j = JsonTools::read_json_file(jsonFile);
    this->scene = Scene();
    if (j.at("type") == TYPE_MENU) {
        for (auto iter = j.at("elements").begin(); iter != j.at("elements").end(); iter++) {
            if ((*iter).at("type") == TYPE_STATIC_IMAGE) {
                this->scene.add_object(StaticImage(*iter));
            } else if ((*iter).at("type") == TYPE_CURSOR) {
                this->scene.add_object(Cursor(*iter), true);
            } else if ((*iter).at("type") == TYPE_BUTTON) {
                this->scene.add_object(Button(*iter));
            }
        }
    } else if (j.at("type") == TYPE_PLAYER) {
        // no implementation
    } else {
        throw std::runtime_error("No valid type found");
    }
}

int main() {
    GameLoop Game;
    Game.init();
    Game.run();
    return 0;
}