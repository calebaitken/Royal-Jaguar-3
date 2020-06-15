/**
 * @author Caleb Aitken
 * @date 10/06/2020
 *
 * Program entry point & main loop header
 *
 * @class GameLoop OOP wrapper for main loop & start-up
 */

#ifndef ROYAL_JAGUAR_3_GAME_H
#define ROYAL_JAGUAR_3_GAME_H

#include <string>
#include "core/Network.h"

// forward declaration
class GameLoop;

/**
 * Wrapper for main loop & start-up
 */
class GameLoop {
public:
    /**
     * Default constructor
     */
    GameLoop() = default;

    /**
     * Default destructor
     */
    ~GameLoop() = default;

    /**
     * Initialise game
     */
    void init();

    /**
     * Main loop
     *
     * init() MUST be called before calling run()
     */
    void run();

private:
    Scene current_scene;
    Network network;
};

#endif //ROYAL_JAGUAR_3_GAME_H
