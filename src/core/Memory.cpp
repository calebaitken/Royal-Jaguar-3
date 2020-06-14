/**
 * @author Caleb Aitken
 * @date 10/06/2020
 *
 * Memory management source
 *
 * TODO:
 *  - Fix error checking methods; assign correct error level (runtime, warn, fatal, etc.)
 *  - Comments
 */

#include "core/Memory.h"

void MemoryManager::update_all() {
    for (const auto& iter : this->objects) {
        try {
            iter.second->update();
        } catch (int e) {
            std::cerr << "Could not update object " << iter.first << std::endl;
        }
    }
}

void MemoryManager::update(unsigned int ID) {
    try {
        this->objects.find(ID)->second->update();
    } catch (int e) {
        std::cerr << "Could not update object " << ID << std::endl;
    }
}

void MemoryManager::free_object(unsigned int ID) {
    try {
        if (this->objects.find(ID)->second.use_count() > 1) {
            std::cerr << "Managed object " << ID << " may not be deleted" << std::endl;
        }

        this->objects.erase(ID);
    } catch (int e) {
        std::cerr << "Could not free object " << ID << std::endl;
    }

    this->nextID.push_front(ID);
}

unsigned int MemoryManager::get_next_id() {
    unsigned int id = this->nextID.front();
    this->nextID.pop_front();

    if (this->nextID.empty()) {
        this->nextID.push_back(id + 1);
    }

    return id;
}