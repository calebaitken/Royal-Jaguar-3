//
// Created by caleb on 2020-03-11.
//

#ifndef ROYAL_JAGUAR_WINDOW_H
#define ROYAL_JAGUAR_WINDOW_H

#include <iostream>
#include <cstdlib>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Window;

class Window {
public:
    explicit Window(int width, int height);
    explicit Window() = default;
    ~Window() = default;

    // non-copyable
    Window(const Window&) = delete;
    Window& operator=(Window&) = delete;

    // movable
    Window(Window&&) = default;
    Window& operator=(Window&&) = default;

    void terminate();
    void resize(int newWidth, int newHeight);

    int get_height();
    int get_width();
    bool get_state();
    glm::mat4 get_projection_mat();

    void set_cursor_pos_callback(GLFWcursorposfun func);

    void clear_buffer();
    void swap_buffer();

private:
    int height;
    int width;
    GLFWwindow* window = nullptr; // TODO: wrap this pointer

    void init();
};


#endif //ROYAL_JAGUAR_WINDOW_H
