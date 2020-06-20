/**
 * @author Caleb Aitken
 * @date 10/06/2020
 *
 * Program entry point & main loop source
 */

#include "core/Game.h"

void GameLoop::setup_game() {
    std::string fake("Empty:;Empty:;Empty:;");
    std::stringstream fakeSocket1(fake, std::ios::in);
    this->scene.reload_scene(fakeSocket1);
    std::stringstream fakeSocket2(fake, std::ios::in);
    this->scene.reload_scene(fakeSocket2);
    std::stringstream fakeSocket3(fake, std::ios::in);
    this->scene.reload_scene(fakeSocket3);
    return;

    char mode;

    do {
        std::cout << "Host or Join (H/J): ";
        std::cin >> mode;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (!(mode == 'H' || mode == 'J'));

    if (mode == 'H') {
        this->network.open_ephemeral();
        std::cout << this->network.get_localhost() << ":" << this->network.get_eph_port() << std::endl;
        // TODO:
        //  create new game state
        //  display ip & port
        //  while not starting game
        //      if connection found
        //          verify, update game state
        //          send to all other connections
        //  send game state to all players
        //      send sizeof list of Objects
        //      send list of objects
        //  play_game
    } else if (mode == 'J') {
        // TODO:
        //  connect to host
        //  verify connection
        //  wait for game state
        //      check special code
        //      get number of objects
        //      create that many objects in scene
        //  play_game
    }
}

/**
 * Main loop
 *
 * TODO: synopsise loop
 */
void GameLoop::play_game() {
    // start graphics
    this->window = Window(0, 0);

    // start input
    this->input.init();

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
            std::array<unsigned int, 2> buffer;
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
 * Entry point
 *
 * @param argv  commandline argument count
 * @param argc  commandline argument values
 * @return      exit status
 */
int main(int argc, char** argv) {
    GameLoop game;
    game.setup_game();
    //game.setup_game();
    //game.play_game();
    return 0;
}