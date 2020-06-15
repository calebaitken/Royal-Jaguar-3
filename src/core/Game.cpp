/**
 * @author Caleb Aitken
 * @date 10/06/2020
 *
 * Program entry point & main loop source
 */

#include "core/Game.h"

/**
 * Main loop
 *
 * TODO: synopsise loop
 */
void GameLoop::run() {
    // while(window should be open)
        // wait for user input (glfwWaitEvents();)
        // begin clock for frame
        // check for network inputs, add to queue
        // check for local inputs, add to queue
        // update all game objects
            // scene object holds all update classes
            // iterate and call update, no returns
        // send network outputs
        // draw
        // stop clock, calculate framerate
    // }
}

/**
 * Initialise game
 *
 * TODO: list init steps
 */
void GameLoop::init() {
    // init networking
    this->network.open_ephemeral();

    // init rendering
        // TODO
    // init scene
    this->current_scene = Scene("resources/scenes/index.json");
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
    return 0;
}