//
// Created by caleb on 12/04/2020.
//

// TODO: find small memory leak when reloading scene. can ignore for now.
// TODO: re-do all constructors

#include "SystemControl.h"
#include "TextRender.h"

void Scene::reset() {
    while (!this->objects.empty()) {
        (*this->objects.begin()).reset(nullptr);
        this->objects.erase(this->objects.begin());
    }

    this->objects.clear();
    this->objects.shrink_to_fit();

    this->_host_menu = false;
    this->_join_menu = false;
}

template <typename T>
void Scene::add_object(std::unique_ptr<T> object) {
    static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
    this->objects.emplace(this->objects.begin(), object.release());
}

template <typename T>
void Scene::add_object(std::unique_ptr<T> object, bool first) {
    static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
    if (!first) {
        this->objects.emplace(this->objects.begin(), object.release());
    } else {
        this->objects.emplace(this->objects.end(), object.release());
    }
}

std::list<std::string> Scene::update_all() {
    std::list<std::string> result, value;
    for (auto position = this->objects.begin(); position != this->objects.end(); position++) {
        result.splice(result.end(), (*position)->update());
    }

    return result;
}

void Scene::draw_all(glm::mat4 projection) {
    for (auto position = this->objects.begin(); position != this->objects.end(); position++) {
        (*position)->draw(projection);
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
    //std::shared_ptr<Font> arial(new Font("src/resources/fonts/arial.ttf"));
    //TextRender text(arial, "this is a test");

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
        //text.draw(this->window.get_projection_mat());
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
        // create gameObjects from elements of scene
        for (auto iter = j.at("elements").begin(); iter != j.at("elements").end(); iter++) {
            if ((*iter).at("type") == TYPE_STATIC_IMAGE) {
                this->scene.add_object(std::unique_ptr<StaticImage>(new StaticImage(*iter)));
            } else if ((*iter).at("type") == TYPE_CURSOR) {
                this->scene.add_object(std::unique_ptr<Cursor>(new Cursor(*iter)), true);
            } else if ((*iter).at("type") == TYPE_IMAGE_BUTTON) {
                this->scene.add_object(std::unique_ptr<ImageButton>(new ImageButton(*iter)));
            } else if ((*iter).at("type") == TYPE_TEXT) {
                //this->scene.add_object(std::unique_ptr<Text>(new Text(*iter)));
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