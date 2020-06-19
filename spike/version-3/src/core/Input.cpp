/**
 * @author Caleb Aitken
 * @date 15/06/2020
 *
 * Input control source
 */

#include "core/Input.h"

void Input::init() {
    GLFWwindow* window = glfwGetCurrentContext();
    assert(window != NULL);
    glfwSetKeyCallback(window, Input::key_callback);
    glfwSetCursorPosCallback(window, Input::cursor_callback);
    Input::initialised = true;
}

void Input::pop(std::array<unsigned int, 2>& data) {
    assert(initialised);
    std::lock_guard<std::mutex> lock(Input::mutex);
    if (Input::queue.empty()) {
        memset(&data, 0, sizeof(data));
    } else {
        data = Input::queue.front();
        Input::queue.pop();
    }
}

std::array<double, 2> Input::get_cursor_pos() {
    assert(initialised);
    std::lock_guard<std::mutex> lock(Input::mutex);
    return std::array<double, 2>(Input::cursorPos);
}

void Input::cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    std::lock_guard<std::mutex> lock(Input::mutex);
    Input::cursorPos = {xpos, ypos};
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // TODO
    if (action != GLFW_PRESS) {
        return;
    }

    std::lock_guard<std::mutex> lock(Input::mutex);

    switch (key) {
        case GLFW_KEY_A:
            std::cout << "a" << std::endl;
            Input::queue.push({1, 1});
            break;
        case GLFW_KEY_B:
            std::cout << "b" << std::endl;
            break;
        case GLFW_KEY_UNKNOWN:
            std::cerr << "unknown key pressed" << std::endl;
            break;
        default:
            break;
    }
}
