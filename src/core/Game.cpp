/**
 * @author Caleb Aitken
 * @date 10/06/2020
 *
 * Program entry point & main loop source
 */

// todo: make all const functions const

#include "core/Game.h"

#include <memory>
#include <objects/Deck.h>
#include <objects/Player.h>

void GameLoop::distribute_state() {
    // fixme: this is too complex
    std::stringstream stateStream(std::ios::app | std::ios::in | std::ios::out | std::ios::binary);
    this->scene.serialise(stateStream);
    for (auto player : this->network.get_connected_sockets()) {
        this->network.write(player, stateStream.str());
    }
}

void GameLoop::setup_game() {
    char mode, cont;

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
        //  create new game state                   HALF-DONE
        //  send game state to all players          DONE
        //  play_game

        int playerCount = 0;

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
                std::cout << "\tPlayer " << this->network.get_IP_of_connected_socket(this->network.get_connected_sockets().back()) << ":" << this->network.get_port_of_connected_socket(this->network.get_connected_sockets().back()) << " connected!" << std::endl;
            }
        }

//        // for each socket in network, send the IP & port to all other sockets
//        for (auto thisSocket : this->network.get_connected_sockets()) {
//            for (auto otherSocket : this->network.get_connected_sockets()) {
//                if (thisSocket != otherSocket) {
//                    this->network.write(otherSocket, this->network.get_IP_of_connected_socket(thisSocket) + ":" + this->network.get_port_of_connected_socket(thisSocket));
//                }
//            }
//        }

        // generate game
        // for each player
        for (int i = 0; i < this->network.get_connected_sockets().size(); i++) {
            std::unique_ptr<Player> newPlayer = std::make_unique<Player>();
            newPlayer->set_socket(this->network.get_connected_sockets().at(i));
            // todo: add cards to the player's deck
            this->scene.add_object(std::move(newPlayer));
        }

        // add yourself too hombre
        std::unique_ptr<Player> self = std::make_unique<Player>();
        this->scene.add_object(std::move(self));

        // tell the world about it
        this->distribute_state();

        // todo: verify it worked

    } else if (mode == 'J') {   // join mode

        std::string hostname;
        std::string port;

        // get hostname and port
        do {
            std::cout << "Enter the hostname: ";
            std::cin >> hostname;
            while (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "ERROR: re-enter hostname: ";
                std::cin >> hostname;
            }

            std::cout << "Enter port: ";
            std::cin >> port;
            while (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "ERROR: re-enter hostname: ";
                std::cin >> port;
            }

        } while (!this->network.connect_to(hostname, port));

        //  todo: verify connection

        //  wait for game state
        std::string buffer;
        for (buffer.clear(); buffer.empty(); this->network.read(this->network.get_connected_sockets().at(0), buffer));
        std::cout << buffer << std::endl;

        // create game state
        std::stringstream stream(std::ios::app | std::ios::out | std::ios::in | std::ios::binary);
        stream.write(reinterpret_cast<const char*>(buffer.c_str()), buffer.length());
        this->scene.reload_scene(stream);


        // todo: check special code
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
    GameLoop game;
    game.setup_game();
    game.play_game();
    return 0;
}