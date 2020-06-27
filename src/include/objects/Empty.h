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

#include <memory>

#include "objects/Object.h"

class Empty final : public Object {
public:
    explicit Empty() = default;
    static std::unique_ptr<Empty> deserialise(std::istream& stream);
    ~Empty() final = default;

    //std::unique_ptr<Empty> Empty(const Empty&) = delete; // TODO: i dont know why i put this here but it broke everything. was there a reason?
    Empty& operator=(Empty&) = delete;

    Empty(Empty&&) = default;
    Empty& operator=(Empty&&) = default;

    void update(std::vector<std::array<unsigned int, 2>> frame) final {};
    void draw(glm::mat4 projection) final {};

    void serialise(std::ostream& stream) const final;
};

#endif //ROYAL_JAGUAR_3_EMPTY_H
