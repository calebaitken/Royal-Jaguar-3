//
// Created by caleb on 17/05/2020.
//

#include "Scene.h"

void Scene::reset() {
    while (!this->jsonObjects.empty()) {
        (*this->jsonObjects.begin()).reset(nullptr);
        this->jsonObjects.erase(this->jsonObjects.begin());
    }

    this->jsonObjects.clear();
    this->jsonObjects.shrink_to_fit();

    while (!this->proceduralObjects.empty()) {
        (*this->proceduralObjects.begin()).reset();
        this->proceduralObjects.erase(this->proceduralObjects.begin());
    }

    this->proceduralObjects.clear();
    this->proceduralObjects.shrink_to_fit();

    this->_host_menu = false;
    this->_join_menu = false;
}

template <typename T>
void Scene::add_json_object(std::unique_ptr<T> object) {
    static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
    this->jsonObjects.emplace(this->jsonObjects.begin(), object.release());
}

template <typename T>
void Scene::add_json_object(std::unique_ptr<T> object, bool first) {
    static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
    if (!first) {
        this->jsonObjects.emplace(this->jsonObjects.begin(), object.release());
    } else {
        this->jsonObjects.emplace(this->jsonObjects.end(), object.release());
    }
}

template <typename T>
void Scene::add_procedural_object(std::shared_ptr<T> object) {
    static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
    this->proceduralObjects.emplace(this->proceduralObjects.begin(), object.release());
}

template <typename T>
void Scene::remove_procedural_object(std::shared_ptr<T> object) {
    static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
    for (auto iter = this->proceduralObjects.begin(); iter != this->proceduralObjects.end(); iter++) {
        if ((*iter) == object) {
            this->proceduralObjects.erase(iter);
        }
    }
}

std::list<std::string> Scene::update_all() {
    std::list<std::string> result, value;
    for (auto position = this->proceduralObjects.begin(); position != this->proceduralObjects.end(); position++) {
        result.splice(result.end(), (*position)->update());
    }

    for (auto position = this->jsonObjects.begin(); position != this->jsonObjects.end(); position++) {
        result.splice(result.end(), (*position)->update());
    }

    return result;
}

void Scene::draw_all(glm::mat4 projection) {
    for (auto position = this->proceduralObjects.begin(); position != this->proceduralObjects.end(); position++) {
        (*position)->draw(projection);
    }

    for (auto position = this->jsonObjects.begin(); position != this->jsonObjects.end(); position++) {
        (*position)->draw(projection);
    }
}