//
// Created by caleb on 12/04/2020.
//

// TODO: verify all constructors

#include "SystemControl.h"

void GameLoop::init() {
    this->window = Window(0, 0);
    this->load_scene(MAIN_MENU_JSON);
}

/**
 * MAIN GAME LOOP
 */
void GameLoop::run() {
    //std::shared_ptr<Font> arial(new Font("src/resources/fonts/arial.ttf"));
    //TextRender text(arial, "this is a test");

    while(this->window.get_state()) {
        // wait for user input
        glfwWaitEvents();

        // begin clock
        auto start = std::chrono::high_resolution_clock::now();

        // get network input
        if (this->_host_menu) {
            if (this->network.accept_connections()) {
                std::unique_ptr<Text> newText(new Text(this->fonts.at("src/resources/fonts/TIMES.TTF"), this->network.get_IP_of_connected_port(*(this->network.get_connected_ports().end() - 1))));
                newText->setPosition(100, 200 * (ResourceManager::getInstance().game_objects_size("HOST MENU") + 1));
                ResourceManager::getInstance().add_game_object("HOST MENU", std::unique_ptr<Text>(newText.release()));
            }
        }

        // update game state
        ResourceManager::getInstance().update_all_game_objects();
        std::pair<std::string, std::string> buffer;
        do {
            ResourceManager::getInstance().pop_object_returns_queue(buffer);
            if (buffer.first == "_load_scene") {
                this->load_scene(buffer.second);
                ResourceManager::getInstance().clear_game_object_queue();
            } else if (buffer.first == "_quit_game") {
                this->window.terminate();
                ResourceManager::getInstance().clear_game_object_queue();
            }
        } while (!(buffer.first.empty() || buffer.second.empty()));


        // send network output

        // draw frame
        this->window.clear_buffer();
        ResourceManager::getInstance().draw_all_game_objects(this->window.get_projection_mat());
        //text.draw(this->window.get_projection_mat());
        this->window.swap_buffer();

        // stop clock and find runtime for loop
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
        // std::cout << duration.count() << std::endl;
    }
}

void GameLoop::load_scene(const std::string& jsonFile) {
    json j = JsonTools::read_json_file(jsonFile);
    ResourceManager::getInstance().reset();
    this->_host_menu = false;
    this->_join_menu = false;
    if (j.at("type") == TYPE_MENU) {
        // create gameObjects from elements of scene
        for (auto iter = j.at("elements").begin(); iter != j.at("elements").end(); iter++) {
            if ((*iter).at("type") == TYPE_STATIC_IMAGE) {
                ResourceManager::getInstance().add_game_object(std::string("STATIC"), std::unique_ptr<StaticImage>(new StaticImage(*iter)));
            } else if ((*iter).at("type") == TYPE_CURSOR) {
                ResourceManager::getInstance().add_game_object(std::string("CURSOR"), std::unique_ptr<Cursor>(new Cursor(*iter)));
            } else if ((*iter).at("type") == TYPE_IMAGE_BUTTON) {
                ResourceManager::getInstance().add_game_object(std::string("STATIC"), std::unique_ptr<ImageButton>(new ImageButton(*iter)));
            } else if ((*iter).at("type") == TYPE_TEXT) {
                auto position = this->fonts.find((*iter).at("font"));
                if (position == this->fonts.end()) {
                    std::shared_ptr<Font> font(new Font((*iter).at("font")));
                    this->fonts.emplace(std::make_pair((*iter).at("font"), font));
                }

                ResourceManager::getInstance().add_game_object(std::string("TEXT"), std::unique_ptr<Text>(new Text(this->fonts.at((*iter).at("font")), (*iter))));
            } else if ((*iter).at("type") == TYPE_TEXT_BUTTON) {
                auto position = this->fonts.find((*iter).at("font"));
                if (position == this->fonts.end()) {
                    std::shared_ptr<Font> font(new Font((*iter).at("font")));
                    this->fonts.emplace(std::make_pair((*iter).at("font"), font));
                }

                ResourceManager::getInstance().add_game_object(std::string("STATIC"), std::unique_ptr<Text>(new TextButton(this->fonts.at((*iter).at("font")), (*iter))));
            }
        }

        // set flags
        for (auto iter = j.at("flags").begin(); iter != j.at("flags").end(); iter++) {
            if ((*iter) == FLAG_HOST_MENU) {
                this->_host_menu = true;
                this->network.open_ephemeral();
                auto position = this->fonts.find(j.at("network").at("IP").at("font"));
                if (position == this->fonts.end()) {
                    std::shared_ptr<Font> font(new Font(j.at("network").at("IP").at("font")));
                    this->fonts.emplace(std::make_pair(j.at("network").at("IP").at("font"), font));
                }

                ResourceManager::getInstance().add_game_object(std::string("TEXT"), std::unique_ptr<Text>(new Text(this->fonts.at(j.at("network").at("IP").at("font")), this->network.get_localhost() + ":" + this->network.get_eph_port(), j.at("network").at("IP"))));
            } else if ((*iter) == FLAG_JOIN_MENU) {
                this->_join_menu = true;
            }
        }
    } else {
        throw std::runtime_error("No valid type found");
    }
}

// TODO: convert most functions to use references to avoid copying. help with memory?
int main() {
    GameLoop Game;
    Game.init();
    Game.run();
    return 0;
}