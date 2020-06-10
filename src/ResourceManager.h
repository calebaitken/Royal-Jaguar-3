//
// Created by caleb on 16/05/2020.
//

#ifndef ROYAL_JAGUAR_3_RESOURCEMANAGER_H
#define ROYAL_JAGUAR_3_RESOURCEMANAGER_H

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "Network.h"
#include "ShaderProgram.h"
#include "GameObject.h"

class ResourceManager {
public:
    // gets instance of class
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    };

    // non-copyable
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&) = delete;

    // non-movable
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

private:
    // private constructor
    ResourceManager() = default;

public:
    void update_all_game_objects();
    void draw_all_game_objects(glm::mat4 projection);
    void update_game_objects(const std::string& gameObjectType);
    void draw_game_objects(const std::string& gameObjectType, glm::mat4 projection);

    std::vector<std::unique_ptr<GameObject>>::iterator game_objects_begin(const std::string& gameObjectType);
    std::vector<std::unique_ptr<GameObject>>::iterator game_objects_end(const std::string& gameObjectType);
    size_t game_objects_size(const std::string& gameObjectType);

    template <typename T>
    void add_game_object(std::string gameObjectType, std::unique_ptr<T> object) {
        static_assert(std::is_base_of<GameObject, T>::value, "T must inherit from GameObject");
        if (this->gameObjects.find(gameObjectType) != this->gameObjects.end()) {
            this->gameObjects.at(gameObjectType).emplace_back(object.release());
        } else {
            this->gameObjects.emplace(gameObjectType, std::vector<std::unique_ptr<GameObject>>());
            this->gameObjects.at(gameObjectType).emplace_back(object.release());
        }
    };

    void delete_game_object(const std::string& gameObjectType, std::vector<std::unique_ptr<GameObject>>::iterator position);

    void reset();

    void pop_object_returns_queue(std::pair<std::string, std::string>& buffer);
    void clear_game_object_queue();

private:
    ThreadSafeQueue<std::pair<std::string, std::string>> objectReturns;
    std::map<std::string, std::vector<std::unique_ptr<GameObject>>> gameObjects;
    //std::map<std::string, std::unique_ptr<Texture2D>> textures;
    //std::map<std::string, std::unique_ptr<Shader>> shaders;
    //std::map<std::string, std::unique_ptr<Font>> fonts;
};


#endif //ROYAL_JAGUAR_3_RESOURCEMANAGER_H
