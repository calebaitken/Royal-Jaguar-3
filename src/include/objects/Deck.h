/**
 * @author Caleb Aitken
 * @date 13/09/2020
 *
 * Deck object header
 */

#ifndef ROYAL_JAGUAR_3_DECK_H
#define ROYAL_JAGUAR_3_DECK_H

#include <memory>
#include <deque>
#include <sstream>
#include <memory.h>
#include <algorithm>
#include <random>
#include <chrono>
#include "objects/Object.h"
#include "objects/Card.h"

// forward declaration
class Deck;

class Deck : public Object {
public:
    /// Default constructor
    explicit Deck() = default;

    /// Deserialisation constructor
    static std::unique_ptr<Deck> deserialise(std::istream& stream);

    /// Default destructor
    ~Deck() override = default;

    /// Deleted copy constructor
    Deck(const Deck&) = delete;

    /// Deleted copy operator
    Deck& operator=(Deck&) = delete;

    /// Default move constructor
    Deck(Deck&&) = default;

    /// Default move operator
    Deck& operator=(Deck&&) = default;

    // TODO
    void draw(glm::mat4 projection) override {};

    // TODO
    void update(std::vector<std::array<unsigned int, 2>> frame) override {};

    void serialise(std::ostream& stream) const override;

    void add_card_front(std::unique_ptr<Card> card);

    void add_card_back(std::unique_ptr<Card> card);

    std::unique_ptr<Card> remove_card_back();

    std::unique_ptr<Card> remove_card_front();

    int size() const;

    /**
     * Randomises the order of elements in the deque
     */
    void shuffle();

private:
    std::deque<std::unique_ptr<Card>> cards;
};

#endif //ROYAL_JAGUAR_3_DECK_H
