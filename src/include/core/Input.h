/**
 * @author Caleb Aitken
 * @date 15/06/2020
 *
 * Input control header
 *
 *                                                 ,  ,
 *                                                / \/ \
 *                                               (/ //_ \_
 *      .-._                                      \||  .  \
 *       \  '-._                            _,:__.-"/---\_ \
 *  ______/___  '.    .--------------------'~-'--.)__( , )\ \
 * `'--.___  _\  /    |                         ,'    \)|\ `\|
 *      /_.-' _\ \ _:,_            HERE BE            " ||   (
 *    .'__ _.' \'-/,`-~`        D R A G O N S           |/
 *        '. ___.> /=,|                                 |
 *         / .-'/_ )  '---------------------------------'
 *    snd  )'  ( /(/
 *              \\ "
 *               '=='
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

class Input;

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
