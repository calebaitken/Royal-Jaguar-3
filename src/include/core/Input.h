/**
 * @author Caleb Aitken
 * @date 15/06/2020
 *
 * Input control header
 */

#ifndef ROYAL_JAGUAR_3_INPUT_H
#define ROYAL_JAGUAR_3_INPUT_H

#include <array>
#include <mutex>
#include <iostream>
#include <memory.h>
#include <queue>
#include <cassert>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

// forward declaration
class Input;
struct CardPosistion;
typedef struct action Action;

/**
 * Plain Old data (POD) // TODO: finish the comment
 *
 */
struct CardPosistion {
    char collection;    // Deck = d; Army = a; Graveyard = g; Hand = h.
    unsigned int index; // position in given collection
};

/**
 * Plain Old Data (POD) // TODO: finish this comment
 *
 * Actions that can be made:
 *  • Playing a card from hand
 *  • Attacking a card
 *  • Ending turn
 */
typedef struct action {
    unsigned int player;            // assign players integers when a game is started
    std::array<char, 2> cardName;
    CardPosistion from;
    CardPosistion to;
} Action;

class Input {
public:
    /// Default constructor
    explicit Input() = default;

    /// Default destructor
    ~Input() = default;

    /// Deleted copy constructor
    Input(const Input&) = delete;

    /// Deleted copy operator
    Input operator=(Input&) = delete;

    /// Default move constructor
    Input(Input&&) = default;

    /// Default move operator
    Input& operator=(Input&&) = delete;

    /**
     *
     * @param data
     */
    void pop(std::array<unsigned int, 2>& data);

    /**
     *
     * @return
     */
    std::array<double, 2> get_cursor_pos();

    /**
     * Initialises the Input class
     */
    void init();

private:
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_callback(GLFWwindow* window, double xpos, double ypos);
    inline static std::queue<std::array<unsigned int, 2>> queue;
    inline static std::array<double, 2> cursorPos;
    inline static std::mutex mutex;

    /// Input must be initialised (using Input::init) AFTER glfw is initialised. Assert will fail if otherwise so
    inline static bool initialised = false;
};

#endif //ROYAL_JAGUAR_3_INPUT_H
