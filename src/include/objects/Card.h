/**
 * @author Caleb Aitken
 * @date 14/06/2020
 *
 * Card header
 */

#ifndef ROYAL_JAGUAR_3_CARD_H
#define ROYAL_JAGUAR_3_CARD_H

#include "core/Memory.h"

class Card;

class Card : public Object {
public:
    explicit Card() = default;
    ~Card() = default;

    Card(const Card&) = delete;
    Card& operator=(Card&) = delete;

    Card(Card&&) = default;
    Card& operator=(Card&&) = default;

    void draw(glm::mat4 projection) override;
    void update() override;

private:
    std::array<char, 2> name;
    int baseDefence;
    int modDefence;
    int baseAttack;
    int modAttack;
};

#endif //ROYAL_JAGUAR_3_CARD_H
