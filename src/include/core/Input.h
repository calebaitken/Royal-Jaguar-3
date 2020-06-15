/**
 * @author Caleb Aitken
 * @date 15/06/2020
 *
 * Input control header
 */

#ifndef ROYAL_JAGUAR_3_INPUT_H
#define ROYAL_JAGUAR_3_INPUT_H

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
};

#endif //ROYAL_JAGUAR_3_INPUT_H
