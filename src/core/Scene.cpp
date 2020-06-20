/**
 * @author Caleb Aitken
 * @date 15/06/2020
 *
 * Object storage control source
 *
 * TODO:
 *  - Fix error checking methods; assign correct error level (runtime, warn, fatal, etc.)
 *  - Comments
 */

#include "core/Scene.h"

/**
 * Updates all stored objects.
 *
 * Iterates through this->objects and calls the virtual update function
 */
void Scene::update_all(std::vector<std::array<unsigned int, 2>> frame) {
    for (const auto& iter : this->objects) {
        try {
            iter.second->update(frame);
        } catch (int e) {
            std::cerr << "Could not update object " << iter.first << std::endl;
        }
    }
}

/**
 * Update specified object
 *
 * @param ID ID of object to update
 */
void Scene::update(unsigned int ID, std::vector<std::array<unsigned int, 2>> frame) {
    try {
        this->objects.find(ID)->second->update(frame);
    } catch (int e) {
        std::cerr << "Could not update object " << ID << std::endl;
    }
}

/**
 * Frees object from memory
 *
 * @param ID Object ID to free
 */
void Scene::free_object(unsigned int ID) {
    try {
        if (this->objects.find(ID)->second.use_count() > 1) {
            std::cerr << "Managed object " << ID << " may not be deleted" << std::endl;
        }

        std::cout << "Freeing object: " << ID << std::endl;
        this->objects.erase(ID);
    } catch (int e) {
        std::cerr << "Could not free object " << ID << std::endl;
    }

    this->nextID.push_front(ID);
}

/**
 * Returns the lowest available ID
 *
 * @return available ID
 */
unsigned int Scene::get_next_id() {
    unsigned int id = this->nextID.front();
    this->nextID.pop_front();

    if (this->nextID.empty()) {
        this->nextID.push_back(id + 1);
    }

    return id;
}

/**
 * Clears scene, then loads objects from a file stream into this scene
 *
 * @param file File to load from
 */
void Scene::reload_scene(std::istream& stream) {
    if (!stream.good()) {
        std::cerr << "stream.good() failed!" << std::endl;
    }

    for (auto object = this->objects.begin(); object != this->objects.end(); object = this->objects.begin()) {
        free_object((object)->first);
    }

    std::string buffer, id, objDelimiter = ";", idDelimiter = ":";
    size_t objend = 0, idend = 0;
    stream >> buffer;
    while ((objend = buffer.find(objDelimiter)) != std::string::npos) {
        std::stringstream obj(buffer.substr(0, objend), std::ios::in);
        if ((idend = buffer.find(idDelimiter)) != std::string::npos) {
            id = buffer.substr(0, idend);
            if (id == "Empty") {
                this->add_object(std::move(Empty::deserialise(obj)));
            }
        }
        buffer.erase(0, objend + objDelimiter.length());
    }

    std::cout << "{";
    for (const auto& object : this->objects) {
        std::cout << "(" << object.first << ", " << object.second << ");";
    }
    std::cout << "}" << std::endl;
}

void Scene::serialise(std::ostream& stream) {
    if (!stream.good()) {
        std::cerr << "stream.good() failed!" << std::endl;
    }

    for (const auto& object : this->objects) {
        object.second->serialise(stream);
        stream << ";";
    }
}
