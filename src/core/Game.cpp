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
    while(window.get_state()) {
        // wait for event
        glfwWaitEvents();

        // begin clock
        auto start = std::chrono::high_resolution_clock::now();

        // get network inputs
        for (auto connection : this->network.get_connected_ports()) {
            std::string buffer;
            this->network.read(connection, buffer);
            // TODO: do something with the data from the connection
            //  - modify Network to automatically accept connections that authenticate
        }

        // get local inputs
        {
            std::array<unsigned int, 2> buffer = {0, 0};
            this->input.pop(buffer);
            // TODO: also do something with buffer
        }

        // update game objects
        //this->scene.update_all()
        // TODO: create data type that can be used to tell each object what inputs have come in?

        // send network outputs
        for (auto connection : this->network.get_connected_ports()) {
            this->network.write(connection, "something");
            // TODO: get information that must be sent to player
        }

        // clean inputs

        // draw
        // this->scene.draw_all(this->window.get_projection_mat());
        // TODO: implement draw function

        // stop clock and find runtime for loop
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        // std::cout << duration.count() << std::endl;
    }
}

/**
 * Initialise game
 *
 * TODO: list init steps
 */
void GameLoop::init() {
    // TODO:
    //  - init networking
    //      this->network.open_ephemeral();

    // TODO:
    //  - init rendering
    //  - init scene

    this->window = Window(0, 0);
    this->input.init();
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
    game.init();
    game.run();
    return 0;
}