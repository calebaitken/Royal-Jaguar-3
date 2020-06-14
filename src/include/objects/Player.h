/**
 * @author Caleb Aitken
 * @date 12/06/2020
 *
 * Player header
 */

#ifndef ROYAL_JAGUAR_3_PLAYER_H
#define ROYAL_JAGUAR_3_PLAYER_H

#include "core/Memory.h"
#include "objects/Card.h"

class Player;

class Player : public Object {
public:
    explicit Player() = default;
    ~Player() = default;

    Player(const Player&) = delete;
    Player& operator=(Player&) = delete;

    Player(Player&&) = default;
    Player& operator=(Player&&) = default;

    void update() override;
    void draw(glm::mat4 projection) override {};

private:
    std::list<Card> hand;
    std::list<Card> army;
    std::deque<Card> graveyard;
    std::deque<Card> deck;
};

#endif //ROYAL_JAGUAR_3_PLAYER_H
