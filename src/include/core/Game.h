/**
 * @author Caleb Aitken
 * @date 10/06/2020
 *
 * Program entry point & main loop header
 *
 * @class GameLoop OOP wrapper for main loop & start-up
 *
 * // TODO: comments
 */

#ifndef ROYAL_JAGUAR_3_GAME_H
#define ROYAL_JAGUAR_3_GAME_H

#include <string>
#include <limits>

#include "core/Input.h"
#include "core/Network.h"
#include "core/Scene.h"
#include "graphics/Window.h"

// forward declaration
class GameLoop;

/**
 * Wrapper for main loop & start-up
 */
class GameLoop {
public:
    /**
     * Constructor calls init()
     */
    GameLoop() = default;

    /**
     * Default destructor
     */
    ~GameLoop() = default;

    /**
     * Main game loop
     *
     * init() MUST be called before calling run()
     */
    void play_game();

    void setup_game();

private:
    void distribute_state();

    /// Handles all networking operations
    Network network;

    /// Stores game state
    Scene scene;

    /// Handles all local inputs
    Input input;

    /// Rendering
    Window window;
};

#endif //ROYAL_JAGUAR_3_GAME_H
