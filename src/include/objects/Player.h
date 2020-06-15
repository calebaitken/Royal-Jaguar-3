/**
 * @author Caleb Aitken
 * @date 12/06/2020
 *
 * Player header
 *
 * @class Player Holds data about a player
 */

#ifndef ROYAL_JAGUAR_3_PLAYER_H
#define ROYAL_JAGUAR_3_PLAYER_H

#define MAX_HEALTH 30
#define DECK_SIZE 54 // TODO: find out the correct number for this

#include "core/Memory.h"
#include "objects/Card.h"

class Player;

/**
 * Object for holding data about player
 */
class Player : public Object {
public:
    /**
     * Player constructor
     */
    explicit Player();

    /**
     * Default player destructor
     */
    ~Player() = default;

    /**
     * Deleted copy constructor
     */
    Player(const Player&) = delete;

    /**
     * Deleted copy operator
     */
    Player& operator=(Player&) = delete;

    /**
     * Default move constructor
     */
    Player(Player&&) = default;

    /**
     * Default move operator
     *
     * @return Reference to moved Player
     */
    Player& operator=(Player&&) = default;

    /**
     * @override
     */
    void update() override;

    /**
     * @override
     */
    void draw(glm::mat4 projection) override {};

private:
    // list of cards in hand
    std::list<Card> hand;

    // list of cards in army (on the board)
    std::list<Card> army;

    // cards in the graveyard
    std::deque<Card> graveyard;

    // cards in the players deck
    std::deque<Card> deck;

    // player's current health
    int currentHealth;
};

#endif //ROYAL_JAGUAR_3_PLAYER_H
