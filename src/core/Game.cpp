/**
 * @author Caleb Aitken
 * @date 10/06/2020
 *
 * Program entry point & main loop source
 */

#include "core/Game.h"

#include <memory>
#include <objects/Deck.h>

void GameLoop::setup_game() {
    char mode, cont;
    int playerCount = 0;

    // get mode
    do {
        std::cout << "Please enter lobby mode; Host or Join (H/J): ";
        std::cin >> mode;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } while (!(mode == 'H' || mode == 'J'));

    if (mode == 'H') {  // host mode
        // TODO:
        //  display ip & port                       DONE
        //  while not starting game                 DONE
        //      if connection found                 DONE
        //          verify, update game state       add verification to Network class, not here
        //          send to all other connections   DONE
        //  create new game state
        //  send game state to all players
        //      send sizeof list of Objects
        //      send list of objects
        //  play_game

        // get the required number of players
        std::cout << "Enter the number of other players waiting to join: ";
        std::cin >> playerCount;
        while(std::cin.fail() || playerCount < 1) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "ERROR: re-enter number of other players waiting to join: ";
            std::cin >> playerCount;
        }

        // open the eph port and display to user
        this->network.open_ephemeral();
        std::cout << this->network.get_localhost() << ":" << this->network.get_eph_port() << std::endl;
        std::cout << "Waiting for players . . . " << std::endl;

        // accept connections until we hit the required amount
        while (this->network.get_connected_sockets().size() < playerCount) {
            if (this->network.accept_connections()) {
                std::cout << "\tPlayer " << this->network.get_IP_of_connected_socket(this->network.get_connected_sockets().back()) << ":"
                << this->network.get_port_of_connected_socket(this->network.get_connected_sockets().back()) << " connected!" << std::endl;
            }
        }

        // for each socket in network, send the IP & port to all other sockets
        for (auto thisSocket : this->network.get_connected_sockets()) {
            for (auto otherSocket : this->network.get_connected_sockets()) {
                if (thisSocket != otherSocket) {
                    this->network.write(otherSocket, this->network.get_IP_of_connected_socket(thisSocket) + ":" + this->network.get_port_of_connected_socket(thisSocket));
                }
            }
        }

        // generate game
        // for each player
            // create player
                // set player number
                // link with socket (?)
            // give cards
        // distribute game state
        // verify

    } else if (mode == 'J') {   // join mode
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
        for (auto connection : this->network.get_connected_sockets()) {
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
        for (auto connection : this->network.get_connected_sockets()) {
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
//    GameLoop game;
//    game.setup_game();
//    game.play_game();
    Scene scene;
    std::unique_ptr<Deck> deck = std::make_unique<Deck>();
    std::stringstream stream(std::ios::app | std::ios::in | std::ios::out | std::ios::binary);
    deck->cards.emplace_front(std::make_unique<Card>());
    scene.add_object(std::make_unique<Card>());
    scene.add_object(std::unique_ptr<Deck>(deck.release()));
    scene.serialise(stream);
    scene.reload_scene(stream);
    return 0;
}