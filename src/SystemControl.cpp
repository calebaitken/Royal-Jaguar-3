//
// Created by caleb on 12/04/2020.
//

// TODO: find small memory leak when reloading scene. can ignore for now.

#include "SystemControl.h"
#include "Network.h"

#define MAIN_MENU_JSON "src/resources/data/Index.json"

void Scene::reset() {
    while (!this->objects.empty()) {
        (*this->objects.begin()).reset(nullptr);
        this->objects.erase(this->objects.begin());
    }

    this->_host_menu = false;
    this->_join_menu = false;
}

std::vector<std::unique_ptr<GameObject>> Scene::get_objects() {
    return std::move(this->objects);
}

template <typename T>
void Scene::add_object(std::unique_ptr<T> object) {
    static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
    this->objects.insert(this->objects.begin(), std::move(object));
}

template <typename T>
void Scene::add_object(std::unique_ptr<T> object, bool first) {
    static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
    if (!first) {
        this->objects.insert(this->objects.begin(), std::move(object));
    } else {
        this->objects.insert(this->objects.end(), std::move(object));
    }
}

std::list<std::string> Scene::update_all() {
    std::list<std::string> result, value;
    for (int i = 0; i < objects.size(); i++) {
        result.splice(result.end(), objects[i].get()->update());
    }

    return result;
}

void Scene::draw_all(glm::mat4 projection) {
    for (int i = 0; i < objects.size(); i++) {
        objects[i].get()->draw(projection);
    }
}

void GameLoop::init() {
    this->window = Window(0, 0);
    this->load_scene(MAIN_MENU_JSON);
}

/**
 * MAIN GAME LOOP
 */
void GameLoop::run() {
    std::list<std::string> gameObjectReturns;

    while(this->window.get_state()) {
        // wait for user input
        glfwWaitEvents();

        // begin clock
        auto start = std::chrono::high_resolution_clock::now();

        // get network input
        // this->network.read_all();

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

        gameObjectReturns.clear();

        // send network output

        // draw frame
        this->window.clear_buffer();
        this->scene.draw_all(this->window.get_projection_mat());
        this->window.swap_buffer();

        // stop clock and find runtime for loop
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        // std::cout << duration.count() << std::endl;
    }
}

void GameLoop::load_scene(const std::string& jsonFile) {
    json j = JsonTools::read_json_file(jsonFile);
    this->scene.reset();
    if (j.at("type") == TYPE_MENU) {
        // create gameobjects from elements of scene
        for (auto iter = j.at("elements").begin(); iter != j.at("elements").end(); iter++) {
            if ((*iter).at("type") == TYPE_STATIC_IMAGE) {
                this->scene.add_object(std::unique_ptr<StaticImage>(new StaticImage(*iter)));
            } else if ((*iter).at("type") == TYPE_CURSOR) {
                this->scene.add_object(std::unique_ptr<Cursor>(new Cursor(*iter)), true);
            } else if ((*iter).at("type") == TYPE_BUTTON) {
                this->scene.add_object(std::unique_ptr<Button>(new Button(*iter)));
            }
        }

        // set flags
        for (auto iter = j.at("flags").begin(); iter != j.at("flags").end(); iter++) {
            if ((*iter) == FLAG_HOST_MENU) {
                this->scene._host_menu = true;
            } else if ((*iter) == FLAG_JOIN_MENU) {
                this->scene._join_menu = true;
            }
        }
    } else {
        throw std::runtime_error("No valid type found");
    }
}

// TODO: convert most functions to use references to avoid copying. help with memory?
int main() {
    GameLoop Game;
    Game.init();
    Game.run();
    return 0;
}