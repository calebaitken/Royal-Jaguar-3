/**
 * @author Caleb Aitken
 * @date 20/06/2020
 *
 * Card object header
 */

#ifndef ROYAL_JAGUAR_3_CARD_H
#define ROYAL_JAGUAR_3_CARD_H

#include "objects/Object.h"

class Card;

class Card : public Object {
public:
    /// Default constructor
    explicit Card() = default;

    /// Default destructor
    ~Card() override = default;

    /// Deleted copy constructor
    Card(const Card&) = delete;

    /// Deleted copy operator
    Card& operator=(Card&) = delete;

    /// Default move constructor
    Card(Card&&) = default;

    /// Default move operator
    Card& operator=(Card&&) = default;

    // TODO
    void draw(glm::mat4 projection) override {};

    // TODO
    void update(std::vector<std::array<unsigned int, 2>> frame) override {};

private:
    std::array<char, 2> name;
    int baseAttack;
    int attackMods;
    int baseDefence;
    int defenceMods;

    // TODO Sprite
};

#endif //ROYAL_JAGUAR_3_CARD_H
