//
// Created by caleb on 16/05/2020.
//

#include "ResourceManager.h"

void ResourceManager::reset() {
    for (auto gameObjectType = this->gameObjects.begin(); gameObjectType != this->gameObjects.end(); gameObjectType++) {
        for (auto gameObject = (*gameObjectType).second.begin(); gameObject != (*gameObjectType).second.end(); gameObject++) {
            (*gameObject).reset();
        }

        (*gameObjectType).second.clear();
        (*gameObjectType).second.shrink_to_fit();
    }

    this->gameObjects.clear();

    this->clear_game_object_queue();
}

void ResourceManager::update_all_game_objects() {
    for (auto& gameObjectType : this->gameObjects) {
        for (auto& gameObject : gameObjectType.second) {
            std::pair<std::string, std::string> buffer;
            buffer = gameObject->update();
            if (!(buffer.first.empty() && buffer.second.empty())) {
                this->objectReturns.push(buffer);
            }
        }
    }
}

void ResourceManager::update_game_objects(const std::string& gameObjectType) {
    for (auto& gameObject : this->gameObjects.at(gameObjectType)) {
        this->objectReturns.push(gameObject->update());
    }
}

void ResourceManager::delete_game_object(const std::string& gameObjectType, std::vector<std::unique_ptr<GameObject>>::iterator position) {
    this->gameObjects.at(gameObjectType).erase(position);
}

std::vector<std::unique_ptr<GameObject>>::iterator ResourceManager::game_objects_begin(const std::string& gameObjectType) {
    return this->gameObjects.at(gameObjectType).begin();
}

std::vector<std::unique_ptr<GameObject>>::iterator ResourceManager::game_objects_end(const std::string& gameObjectType) {
    return this->gameObjects.at(gameObjectType).end();
}

size_t ResourceManager::game_objects_size(const std::string& gameObjectType) {
    auto position = this->gameObjects.find(gameObjectType);
    if (position == this->gameObjects.end()) {
        std::cerr << "Game Object Type does not exist. Defaulting to size 0." << std::endl;
        return 0;
    }

    return this->gameObjects.at(gameObjectType).size();
}

void ResourceManager::pop_object_returns_queue(std::pair<std::string, std::string>& buffer) {
    this->objectReturns.pop(buffer);
}

void ResourceManager::draw_all_game_objects(glm::mat4 projection) {
    // draw everything
    for (auto& gameObjectType : this->gameObjects) {
        for (auto& gameObject : gameObjectType.second) {
            gameObject->draw(projection);
        }
    }

    // draw cursors
    auto cursorList = this->gameObjects.find("CURSOR");
    if (cursorList != this->gameObjects.end()) {
        for (auto& cursor : (*cursorList).second) {
            cursor->draw(projection);
        }
    }
}

void ResourceManager::draw_game_objects(const std::string& gameObjectType, glm::mat4 projection) {
    auto position = this->gameObjects.find(gameObjectType);
    if (position == this->gameObjects.end()) {
        std::cerr << "Game Object Type does not exist" << std::endl;
        return;
    }

    for (auto& gameObject : this->gameObjects.at(gameObjectType)) {
        gameObject->draw(projection);
    }
}

void ResourceManager::clear_game_object_queue() {
    std::pair<std::string, std::string> buffer;
    do {
        this->objectReturns.pop(buffer);
    } while (!(buffer.first.empty() || buffer.second.empty()));
}