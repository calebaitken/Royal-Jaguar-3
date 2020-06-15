/**
 * @author Caleb Aitken
 * @date 15/06/2020
 *
 * Object storage control header.
 *
 * @class Scene Control of game resources in scene
 *
 * TODO:
 *  - Error checking
 */

#ifndef ROYAL_JAGUAR_3_SCENE_H
#define ROYAL_JAGUAR_3_SCENE_H

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <deque>
#include "objects/Object.h"

// forward declaration
class Scene;

class Scene {
public:
    /// Default constructor; use reload_scene to initialise a scene
    explicit Scene() = default;

    /// Default destructor
    ~Scene() = default;

    /// Deleted copy constructor
    Scene(const Scene&) = delete;

    /// Deleted copy operator
    Scene& operator=(Scene&) = delete;

    /// Default move constructor
    Scene(Scene&&) = default;

    /// Default move operator
    Scene& operator=(Scene&&) = default;

    /**
     * Clears scene, then loads objects from a file into this object
     *
     * @param file File to load from
     */
    void reload_scene(std::string file);

    // TODO:
    //  - void draw_all()
    //  - void draw(unsigned int ID);

    /**
     * Update all stored objects.
     */
    void update_all();

    /**
     * Update specified object
     *
     * @param ID ID of object to update
     */
    void update(unsigned int ID);

    /**
     * Moves Object to globally access location.
     *
     * @tparam T Type of data to store. Must inherit from Object.
     * @param object Unique_ptr to move into global location. Must be passed into function using std::unique_ptr::release
     * @return Generated ID of object. Used to directly reference to the object.
     */
    template<typename T>
    unsigned int add_object(std::unique_ptr<T> object) {
        static_assert(std::is_base_of<Object, T>::value, "T must inherit from Object");
        unsigned int ID = get_next_id();
        this->objects.emplace(ID, std::move(object));
        return ID;
    }

    /**
     * Returns a weak_ptr to the object found at the given ID.
     *
     * @tparam T Requested type of data. Must inherit from Object.
     * @param ID Object ID to find.
     * @return weak_ptr or null.
     */
    template<typename T>
    std::weak_ptr<T> get_object(unsigned int ID) {
        static_assert(std::is_base_of<Object, T>::value, "T must inherit from Object");
        return std::dynamic_pointer_cast<T>(this->objects.at(ID));
    } // TODO

    /**
     * Frees object from memory
     *
     * @param ID Object ID to free
     */
    void free_object(unsigned int ID);

private:
    /**
     * Returns the lowest available ID
     *
     * @return available ID
     */
    unsigned int get_next_id();

    /// stores all objects in a map
    std::map<unsigned int, std::shared_ptr<Object>> objects;

    /// for distributing IDs
    std::deque<unsigned int> nextID = std::deque<unsigned int>(1, 0);
};

#endif //ROYAL_JAGUAR_3_SCENE_H
