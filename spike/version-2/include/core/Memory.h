/**
 * @author Caleb Aitken
 * @date 10/06/2020
 *
 * Memory management header
 *
 * @class MemoryManager Singleton object for global control of game resources
 * @class Object Virtual interface for globally managed objects
 *
 * TODO:
 *  - Comment Object
 *  - Error checking
 */

#ifndef ROYAL_JAGUAR_3_MEMORY_H
#define ROYAL_JAGUAR_3_MEMORY_H

#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <list>
#include <deque>
#include <glm/glm.hpp>

// forward declarations
class MemoryManager;
class Object;

/**
 * Virtual interface for globally managed objects
 *
 * non-copyable, movable
 */
class Object {
public:
    explicit Object() = default;
    virtual ~Object() = default;

    Object(const Object&) = delete;
    Object& operator=(Object&) = delete;

    Object(Object&&) = default;
    Object& operator=(Object&&) = default;

    virtual void draw(glm::mat4 projection) = 0;
    virtual void update() = 0;
};

/**
 * Singleton object for global control of game resources.
 *
 * Must be accessed via the getInstance() method.
 *
 * MemoryManager is non-copyable & non-movable to prevent duplicate instances.
 * Lazy initialisation.
 */
class MemoryManager {
public:
    /**
     * Singleton access for class functions
     *
     * @return reference to MemoryManager singleton
     */
    static MemoryManager& getInstance() {
        // lazy init of instance
        static MemoryManager instance;
        return instance;
    };

    /**
     * MemoryManager copy constructor deleted (non-copyable)
     */
    MemoryManager(const MemoryManager&) = delete;

    /**
     * MemoryManager copy operator deleted (non-copyable)
     */
    MemoryManager& operator=(MemoryManager&) = delete;

    /**
     * MemoryManager move constructor deleted (non-movable)
     */
    MemoryManager(MemoryManager&&) = delete;

    /**
     * MemoryManager move operator deleted (non-movable)
     */
    MemoryManager& operator=(MemoryManager&&) = delete;

private:
    /**
     * MemoryManager constructor made private to prevent duplicate instances
     */
    MemoryManager() = default;

public:
    // void draw_all(); // TODO

    /**
     * Update all known objects.
     */
    void update_all();

    /**
     * Update specified object
     *
     * @param type ID of object to update
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

    // stores all objects in a map
    std::map<unsigned int, std::shared_ptr<Object>> objects;

    // for distributing IDs
    std::deque<unsigned int> nextID = std::deque<unsigned int>(1, 0);
};
#endif //ROYAL_JAGUAR_3_MEMORY_H
