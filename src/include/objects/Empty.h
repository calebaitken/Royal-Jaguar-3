/**
 * @author Caleb Aitken
 * @date 10/06/2020
 *
 * Empty
 *
 * @class Empty Inherits from Object. Its empty.
 */

#ifndef ROYAL_JAGUAR_3_EMPTY_H
#define ROYAL_JAGUAR_3_EMPTY_H

#include "core/Memory.h"

class Empty final : public Object {
public:
    explicit Empty() = default;
    ~Empty() final = default;

    Empty(const Empty&) = delete;
    Empty& operator=(Empty&) = delete;

    Empty(Empty&&) = default;
    Empty& operator=(Empty&&) = delete;

    void update() final {};
    void draw(glm::mat4 projection) final {};
};

#endif //ROYAL_JAGUAR_3_EMPTY_H
