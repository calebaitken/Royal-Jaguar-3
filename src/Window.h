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

class Window;

class Window {
private:
    int height;
    int width;
    GLFWwindow* window = nullptr;

public:
    Window(int width, int height) {
        this->width = width;
        this->height = height;
        if (this->window == nullptr) {
            this->init();
        } else {
            this->resize(width, height);
        }
    };

    Window() = default;
    ~Window() = default;

    int get_height() {
        return this->height;
    }

    int get_width() {
        return this->width;
    }

    void resize(int width, int height) {
        this->width = width;
        this->height = height;
        glfwSetWindowSize(this->window, this->width, this->height);
    }

    void terminate() {
        if (this->window != nullptr) {
            glfwSetWindowShouldClose(this->window, GLFW_TRUE);
        } else {
            std::cerr << "GLFW not initalised\n";
        }
    }

    void clear_buffer() {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void swap_buffer() {
        glfwSwapBuffers(this->window);
    }

    bool get_state() {
        return !glfwWindowShouldClose(this->window);
    }

    glm::mat4 get_projection_mat() {
        return glm::ortho(0.0f, static_cast<GLfloat>(this->width), static_cast<GLfloat>(this->height), 0.0f, -1.0f, 1.0f);
    }

    void set_cursor_pos_callback(GLFWcursorposfun func) {
        glfwSetCursorPosCallback(this->window, func);
    }

private:
    void init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        if (this->width == 0 && this->height == 0) {
            this->width = vidmode->width;
            this->height = vidmode->height;
            glfwWindowHint(GLFW_RED_BITS, vidmode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, vidmode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, vidmode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
            glfwWindowHint(GLFW_AUTO_ICONIFY, GL_TRUE);
        }

        this->window = glfwCreateWindow(this->width, this->height, "Royal Jaguar", glfwGetPrimaryMonitor(), NULL);
        if (this->window == nullptr) {
            glfwTerminate();
            throw new std::runtime_error("Failed to create window");
        }

        //glfwSetWindowPos(this->window,
                         //(vidmode->width - this->width) / 2,
                         //(vidmode->height - this->height) / 2);

        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        //glfwSetInputMode(this->window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

        if (glfwRawMouseMotionSupported()) {
            std::cout << "raw motion" << std::endl;
            glfwSetInputMode(this->window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        }

        glfwMakeContextCurrent(this->window);

        glewInit();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glfwSwapInterval(1);

        glfwShowWindow(this->window);
    };
};


#endif //ROYAL_JAGUAR_WINDOW_H
