//
// Created by caleb on 7/11/2020.
//

#ifndef ROYAL_JAGUAR_3_PLAYER_H
#define ROYAL_JAGUAR_3_PLAYER_H

#include <memory>
#include "objects/Object.h"
#include "objects/Deck.h"

// forward declaration
class Player;

class Player : public Object {
public:
    /// Default constructor
    explicit Player() = default;

    /// Deserialisation constructor
    static std::unique_ptr<Player> deserialise(std::istream& stream);

    /// Default destructor
    ~Player() override = default;

    /// Deleted copy constructor
    Player(const Player&) = delete;

    /// Deleted copy operator
    Player& operator=(Player&) = delete;

    /// Default move constructor
    Player(Player&&) = default;

    /// Default move operator
    Player& operator=(Player&&) = default;

    // TODO
    void draw(glm::mat4 projection) override {};

    // TODO
    void update(std::vector<std::array<unsigned int, 2>> frame) override {};

    void serialise(std::ostream& stream) const override;

    std::shared_ptr<const Deck> get_deck();

private:
    std::shared_ptr<Deck> deck;
};

#endif //ROYAL_JAGUAR_3_PLAYER_H
