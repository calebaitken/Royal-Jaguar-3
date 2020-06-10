//
// Created by caleb on 17/05/2020.
//

#ifndef ROYAL_JAGUAR_3_SCENE_H
#define ROYAL_JAGUAR_3_SCENE_H

class Scene;

class Scene {
public:
    Scene() = default;
    ~Scene() = default;

    void reset();

    std::list<std::string> update_all();
    void draw_all(glm::mat4 projection);
    template <typename T> void add_json_object(std::unique_ptr<T> object);
    template <typename T> void add_json_object(std::unique_ptr<T> object, bool first);

    template <typename T> void add_procedural_object(std::shared_ptr<T> object);
    template <typename T> void remove_procedural_object(std::shared_ptr<T> object);

    bool _host_menu = false;
    bool _join_menu = false;

private:
    std::vector<std::unique_ptr<GameObject>> jsonObjects;
    std::vector<std::shared_ptr<GameObject>> proceduralObjects;
};

#endif //ROYAL_JAGUAR_3_SCENE_H
