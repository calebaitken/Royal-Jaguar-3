/**
 * @author Caleb Aitken
 * @date 10/06/2020
 *
 * Program entry point & main loop source
 */

#include "core/Game.h"
#include "core/Memory.h"
#include "objects/Empty.h"

/**
 * Main loop
 *
 * TODO: synopsise loop
 */
void GameLoop::run() {

}

/**
 * Initialise game
 *
 * TODO: list init steps
 */
void GameLoop::init() {

}

/**
 * Entry point
 *
 * @param argv  commandline argument count
 * @param argc  commandline argument values
 * @return      exit status
 */
int main(int argc, char** argv) {
    GameLoop game;
    //game.init();
    //game.run();
    std::cout << MemoryManager::getInstance().add_object(std::unique_ptr<Empty>(new Empty())) << std::endl;
    std::cout << MemoryManager::getInstance().add_object(std::unique_ptr<Empty>(new Empty())) << std::endl;
    std::cout << MemoryManager::getInstance().add_object(std::unique_ptr<Empty>(new Empty())) << std::endl;
    std::cout << MemoryManager::getInstance().add_object(std::unique_ptr<Empty>(new Empty())) << std::endl;
    std::cout << MemoryManager::getInstance().add_object(std::unique_ptr<Empty>(new Empty())) << std::endl;
    std::cout << MemoryManager::getInstance().add_object(std::unique_ptr<Empty>(new Empty())) << std::endl;
    MemoryManager::getInstance().free_object(1);
    std::cout << MemoryManager::getInstance().add_object(std::unique_ptr<Empty>(new Empty())) << std::endl;
    std::cout << MemoryManager::getInstance().add_object(std::unique_ptr<Empty>(new Empty())) << std::endl;
    std::cout << MemoryManager::getInstance().get_object<Empty>(2).use_count() << std::endl;
    return 0;
}