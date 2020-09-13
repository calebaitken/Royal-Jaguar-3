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

#include <memory>
#include <objects/Deck.h>

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

    unsigned int nextReadSize = 0;
    char* buffer = (char*) calloc(1024, sizeof(char));
    std::string objType;
    std::stringstream subStream(std::ios::app | std::ios::out | std::ios::in | std::ios::binary);
    do {
        std::cout << std::endl << "Now extracting next extract size . . . ";
        stream.read(reinterpret_cast<char*>(&nextReadSize), sizeof(unsigned int));
        if (stream.eof()) {
            std::cout << "EOF" << std::endl << std::endl;
            break;
        } else if (stream.fail()) {
            std::cout << "FAILED" << std::endl << std::endl;
            std::cerr << "Extracting size of next read failed" << std::endl;
            break;
        }
        std::cout << "SUCCESS" << std::endl;

        std::cout << "Now extracting " << nextReadSize << " bytes . . . ";
        memset(&buffer[0], 0, sizeof(buffer));
        stream.read(reinterpret_cast<char*>(buffer), nextReadSize);
        if (stream.fail()) {
            std::cout << "FAILED" << std::endl;
            std::cerr << "Extracting next object type failed" << std::endl;
            break;
        }
        std::cout << "SUCCESS" << std::endl;

        objType.assign(buffer);

        std::cout << "Now extracting size of " << objType << " . . . ";
        stream.read(reinterpret_cast<char*>(&nextReadSize), sizeof(unsigned int));
        if (stream.fail()) {
            std::cout << "FAILED" << std::endl;
            std::cerr << "Extracting size of object failed" << std::endl;
            break;
        }
        std::cout << "SUCCESS" << std::endl;

        std::cout << "Now extracting " << nextReadSize << " bytes . . . ";
        memset(&buffer[0], 0, sizeof(buffer));
        stream.read(reinterpret_cast<char*>(buffer), nextReadSize);
        if (stream.fail()) {
            std::cout << "FAILED" << std::endl;
            std::cerr << "Extracting object failed" << std::endl;
            break;
        }
        std::cout << "SUCCESS" << std::endl;

        std::cout << "Creating substream of size " << nextReadSize << " . . . ";
        subStream.write(reinterpret_cast<const char*>(&buffer[0]), nextReadSize);
        std::cout << "SUCCESS" << std::endl;

        std::cout << "Appending " << objType << " to scene . . . " << std::endl;
        if (objType == "Empty") {
            this->add_object(std::move(Empty::deserialise(subStream)));
        } else if (objType == "Card") {
            this->add_object(std::move(Card::deserialise(subStream)));
        } else if (objType == "Deck") {
            this->add_object(std::move(Deck::deserialise(subStream)));
        }
        std::cout << "DONE" << std::endl;

    } while (!stream.eof());

    std::cout << "{";
    for (const auto& object : this->objects) {
        std::cout << "(" << object.first << ", " << object.second << ");";
    }
    std::cout << "}" << std::endl;

    free(buffer);
}

void Scene::serialise(std::ostream& stream) {
    if (!stream.good()) {
        std::cerr << "stream.good() failed!" << std::endl;
    }

    for (const auto& object : this->objects) {
        object.second->serialise(stream);
        //stream << ";"; FIXME: removed because this should be binary stream?
    }
}
