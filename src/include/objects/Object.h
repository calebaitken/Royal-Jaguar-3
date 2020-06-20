/**
 * @author Caleb Aitken
 * @date 15/06/2020
 *
 * Virtual object class
 *
 * @class Object Virtual interface for managed objects
 */

#ifndef ROYAL_JAGUAR_3_OBJECT_H
#define ROYAL_JAGUAR_3_OBJECT_H

#include <vector>
#include <array>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

// forward declaration
class Object;

/**
 * Virtual interface for globally managed objects
 *
 * non-copyable, movable
 */
class Object {
public:
    /// Default constructor
    explicit Object() = default;

    /// Virtual default destructor
    virtual ~Object() = default;

    /// Deleted copy constructor
    Object(const Object&) = delete;

    /// Deleted copy operator
    Object& operator=(Object&) = delete;

    /// Default move constructor
    Object(Object&&) = default;

    /// Default move operator
    Object& operator=(Object&&) = default;

    /**
     * OpenGL instructions for drawing all elements of object
     *
     * @param projection Projection matrix of screen
     */
    virtual void draw(glm::mat4 projection) = 0;

    /**
     * Virtual function called each loop
     */
    virtual void update(std::vector<std::array<unsigned int, 2>> frame) = 0;

    /**
     * Writes this object in binary form to the given ostream
     *
     * @param ostream Stream to write to
     */
    virtual void serialise(std::ostream& stream) const = 0;
};

#endif //ROYAL_JAGUAR_3_OBJECT_H
