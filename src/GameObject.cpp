//
// Created by caleb on 14/04/2020.
//


#include "GameObject.h"

void GameObject::draw(glm::mat4 projection) {
    this->sprite->draw(projection);
}

StaticImage::StaticImage(const std::string& imageFile) {
    this->sprite.reset(new SpriteRender(imageFile));
}

StaticImage::StaticImage(const json& j) {
    //std::cout << j.dump() << std::endl;

    this->sprite.reset(new SpriteRender(j.at("image")));

    // SET POSITION
    // set sprite x-position
    if (j.at("xpos") == 0) {
        // do nothing
    } else if (j.at("xpos") < 1) {
        // set according to ratio
        this->sprite->setPosition(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, this->sprite->getPosition()[1]);
        this->sprite->setPosition(j.at("xpos"), 1, true);
    } else {
        // set exact
        this->sprite->setPosition(j.at("xpos"), this->sprite->getPosition()[1]);
    }

    // set sprite y-position
    if (j.at("ypos") == 0) {
        // do nothing
    } else if (j.at("ypos") < 1) {
        // set according to ratio
        this->sprite->setPosition(this->sprite->getPosition()[0], glfwGetVideoMode(glfwGetPrimaryMonitor())->height);
        this->sprite->setPosition(1, j.at("ypos"), true);
    } else {
        // set exact
        this->sprite->setPosition(this->sprite->getPosition()[0], j.at("ypos"));
    }

    // SET SIZE
    // set sprite x-size
    if (j.at("xsize") == 0) {
        // do nothing
    } else if (j.at("xsize") < 1) {
        // set according to ratio
        this->sprite->setSize(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, this->sprite->getSize()[1]);
        this->sprite->setSize(j.at("xsize"), 1, true);
    } else {
        // set exact
        this->sprite->setSize(j.at("xsize"), this->sprite->getSize()[1]);
    }

    // set sprite y-size
    if (j.at("ysize") == 0) {
        // do nothing
    } else if (j.at("ysize") < 1) {
        // set according to ratio
        this->sprite->setSize(this->sprite->getSize()[0], glfwGetVideoMode(glfwGetPrimaryMonitor())->height);
        this->sprite->setSize(1, j.at("ysize"), true);
    } else {
        // set exact
        this->sprite->setSize(this->sprite->getSize()[0], j.at("ysize"));
    }

    // HANDLE FLAGS
    for (auto iter = j.at("flags").begin(); iter != j.at("flags").end(); iter++) {
        if ((*iter) == "scaleToWidth") {
            this->sprite->scale_to_width();
        }
    }
}

Cursor::Cursor(const std::string& imageFile) {
    this->sprite.reset(new SpriteRender(imageFile));
    this->update();
}

Cursor::Cursor(const json &j) {
    //std::cout << j.dump() << std::endl;
    for (auto iter = j.at("elements").begin(); iter != j.at("elements").end(); iter++) {
        if ((*iter).at("type") == "_pointer") {
            this->sprite.reset(new SpriteRender((*iter).at("image")));
        } else if ((*iter).at("type") == "_hand") {
            this->altSprite.reset(new SpriteRender((*iter).at("image")));
        }
    }

    //this->sprite.setSize(15.0f, 23.7f);
    this->altSprite->setSize(20.0f, 31.0f);
    this->update();
}

void Cursor::draw(glm::mat4 projection) {
    if (drawAlt) {
        this->altSprite->draw(projection);
    } else {
        this->sprite->draw(projection);
    }
}

std::list<std::string> Cursor::update() {
    double xpos, ypos;
    glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);

    if (xpos <= 0) {
        glfwSetCursorPos(glfwGetCurrentContext(), 0, ypos);
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
    }
    if (xpos >= glfwGetVideoMode(glfwGetPrimaryMonitor())->width) {
        glfwSetCursorPos(glfwGetCurrentContext(), glfwGetVideoMode(glfwGetPrimaryMonitor())->width, ypos);
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
    }
    if (ypos <= 0) {
        glfwSetCursorPos(glfwGetCurrentContext(), xpos, 0);
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
    }
    if (ypos >= glfwGetVideoMode(glfwGetPrimaryMonitor())->height) {
        glfwSetCursorPos(glfwGetCurrentContext(), xpos, glfwGetVideoMode(glfwGetPrimaryMonitor())->height);
        glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
    }

    this->sprite->setPosition(xpos, ypos);
    this->altSprite->setPosition(xpos, ypos);

    return {};
}

ImageButton::ImageButton(const json &j) : StaticImage(j) {
    for (auto iter = j.at("function").begin(); iter != j.at("function").end(); iter++) {
        function.emplace_back((*iter));
    }
}

std::list<std::string> ImageButton::update() {
    double xpos, ypos;
    glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);

    if (xpos > this->sprite->getPosition()[0] && xpos < (this->sprite->getPosition()[0] + this->sprite->getSize()[0]) &&
        ypos > this->sprite->getPosition()[1] && ypos < (this->sprite->getPosition()[1] + this->sprite->getSize()[1])) {
        this->sprite->setColour(0.7f, 0.7f, 0.7f);
        if (waiting_release) {
            if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
                waiting_release = false;
                return function;
            }
        } else {
            if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                waiting_release = true;
            }
        }
    } else {
        waiting_release = false;
        this->sprite->setColour(1.0f, 1.0f, 1.0f);
    }

    return {};
}


Text::Text(const std::shared_ptr<Font>& font, const std::string &string) {
    this->text.reset(new TextRender(font, string));
}

Text::Text(const std::shared_ptr<Font>& font, const json& j) {
    std::cout << j.dump() << std::endl;

    this->text.reset(new TextRender(font, j.at("text")));

    // SET POSITION
    // set text x-position
    if (j.at("xpos") == 0) {
        // do nothing
    } else if (j.at("xpos") < 1) {
        // set according to ratio
        this->text->setPosition(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, this->text->getPosition()[1]);
        this->text->setPosition(j.at("xpos"), 1, true);
    } else {
        // set exact
        this->text->setPosition(j.at("xpos"), this->text->getPosition()[1]);
    }

    // set text y-position
    if (j.at("ypos") == 0) {
        // do nothing
    } else if (j.at("ypos") < 1) {
        // set according to ratio
        this->text->setPosition(this->text->getPosition()[0], glfwGetVideoMode(glfwGetPrimaryMonitor())->height);
        this->text->setPosition(1, j.at("ypos"), true);
    } else {
        // set exact
        this->text->setPosition(this->text->getPosition()[0], j.at("ypos"));
    }

    // SET SIZE
    // set text x-size
    if (j.at("xsize") == 0) {
        // do nothing
    } else if (j.at("xsize") < 1) {
        // set according to ratio
        this->text->setSize(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, this->text->getSize()[1]);
        this->text->setSize(j.at("xsize"), 1, true);
    } else {
        // set exact
        this->text->setSize(j.at("xsize"), this->text->getSize()[1]);
    }

    // set text y-size
    if (j.at("ysize") == 0) {
        this->text->setSize(this->text->getSize()[0], 0);
    } else if (j.at("ysize") < 1) {
        // set according to ratio
        this->text->setSize(this->text->getSize()[0], glfwGetVideoMode(glfwGetPrimaryMonitor())->height);
        this->text->setSize(1, j.at("ysize"), true);
    } else {
        // set exact
        this->text->setSize(this->text->getSize()[0], j.at("ysize"));
    }

    // HANDLE FLAGS
    for (auto iter = j.at("flags").begin(); iter != j.at("flags").end(); iter++) {
        // TODO: make flags if needed
    }
}

Text::Text(const std::shared_ptr<Font>& font, const std::string& string, const json& j) {
    std::cout << j.dump() << std::endl;

    this->text.reset(new TextRender(font, string));

    // SET POSITION
    // set text x-position
    if (j.at("xpos") == 0) {
        // do nothing
    } else if (j.at("xpos") < 1) {
        // set according to ratio
        this->text->setPosition(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, this->text->getPosition()[1]);
        this->text->setPosition(j.at("xpos"), 1, true);
    } else {
        // set exact
        this->text->setPosition(j.at("xpos"), this->text->getPosition()[1]);
    }

    // set text y-position
    if (j.at("ypos") == 0) {
        // do nothing
    } else if (j.at("ypos") < 1) {
        // set according to ratio
        this->text->setPosition(this->text->getPosition()[0], glfwGetVideoMode(glfwGetPrimaryMonitor())->height);
        this->text->setPosition(1, j.at("ypos"), true);
    } else {
        // set exact
        this->text->setPosition(this->text->getPosition()[0], j.at("ypos"));
    }

    // SET SIZE
    // set text x-size
    if (j.at("xsize") == 0) {
        // do nothing
    } else if (j.at("xsize") < 1) {
        // set according to ratio
        this->text->setSize(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, this->text->getSize()[1]);
        this->text->setSize(j.at("xsize"), 1, true);
    } else {
        // set exact
        this->text->setSize(j.at("xsize"), this->text->getSize()[1]);
    }

    // set text y-size
    if (j.at("ysize") == 0) {
        this->text->setSize(this->text->getSize()[0], 0);
    } else if (j.at("ysize") < 1) {
        // set according to ratio
        this->text->setSize(this->text->getSize()[0], glfwGetVideoMode(glfwGetPrimaryMonitor())->height);
        this->text->setSize(1, j.at("ysize"), true);
    } else {
        // set exact
        this->text->setSize(this->text->getSize()[0], j.at("ysize"));
    }

    // HANDLE FLAGS
    for (auto iter = j.at("flags").begin(); iter != j.at("flags").end(); iter++) {
        // TODO: make flags if needed
    }
}


void Text::draw(glm::mat4 projection) {
    this->text->draw(projection);
}

std::list<std::string> Text::update() {
    return {};
}

TextButton::TextButton(const std::shared_ptr<Font>& font, const json& j) : Text(font, j) {
    for (auto iter = j.at("function").begin(); iter != j.at("function").end(); iter++) {
        function.emplace_back((*iter));
    }
}

std::list<std::string> TextButton::update() {
    double xpos, ypos;
    glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);

    if (xpos > this->text->getPosition()[0] && xpos < (this->text->getPosition()[0] + this->text->getSize()[0]) &&
        ypos < this->text->getPosition()[1] && ypos > (this->text->getPosition()[1] - this->text->getSize()[1])) {
        this->text->setColour(0.7f, 0.7f, 0.7f);
        if (waiting_release) {
            if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
                waiting_release = false;
                return function;
            }
        } else {
            if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                waiting_release = true;
            }
        }
    } else {
        waiting_release = false;
        this->text->setColour(1.0f, 1.0f, 1.0f);
    }

    return {};
}