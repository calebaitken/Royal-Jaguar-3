//
// Created by caleb on 13/09/2020.
//

#include "objects/Deck.h"

void Deck::serialise(std::ostream &stream) const {
    if (!stream.good()) {
        std::cerr << "ostream.good() failed" << std::endl;
    }

    std::string className = "Deck";
    unsigned int classNameSize, objSize;
    classNameSize = className.size();
    // Card Header: int, char[4], int
    // Card Data: int, char[2], int, int, int, int
    objSize = ((sizeof(int) * 4) + (2 + sizeof(unsigned int)) + (2 * sizeof(unsigned int)) + 4) * cards.size();

    std::cout << "Serialising Deck . . . ";
    // header
    stream.write(reinterpret_cast<const char*>(&classNameSize), sizeof(unsigned int));
    stream.write(reinterpret_cast<const char*>(className.c_str()), classNameSize);
    stream.write(reinterpret_cast<const char*>(&objSize), sizeof(unsigned int));

    // data
    for (const auto& card : this->cards) {
        std::cout.setstate(std::ios_base::failbit);
        card->serialise(stream);
        std::cout.clear();
    }
    std::cout << "DONE" << std::endl;
}

std::unique_ptr<Deck> Deck::deserialise(std::istream &stream) {
    if (!stream.good()) {
        std::cerr << "istream.good() failed" << std::endl;
    }

    Deck returnDeck;

    unsigned int nextReadSize = 0;
    char* buffer = (char*) calloc(1024, sizeof(char));
    std::string objType;
    std::stringstream subStream(std::ios::app | std::ios::out | std::ios::in | std::ios::binary);
    do {
        std::cout << "\tNow extracting next extract size . . . ";
        stream.read(reinterpret_cast<char*>(&nextReadSize), sizeof(unsigned int));
        if (stream.eof()) {
            std::cout << "EOF" << std::endl << std::endl;
            break;
        } else if (stream.fail()) {
            std::cout << "FAILED" << std::endl << std::endl;
            std::cerr << "Extracting size of next read failed" << std::endl;
            break;
        }
        std::cout << "SUCCESS" << std::endl;

        std::cout << "\tNow extracting " << nextReadSize << " bytes . . . ";
        memset(&buffer[0], 0, sizeof(buffer));
        stream.read(reinterpret_cast<char*>(buffer), nextReadSize);
        if (stream.fail()) {
            std::cout << "FAILED" << std::endl;
            std::cerr << "Extracting next object type failed" << std::endl;
            break;
        }
        std::cout << "SUCCESS" << std::endl;

        objType.assign(buffer);

        std::cout << "\tNow extracting size of " << objType << " . . . ";
        stream.read(reinterpret_cast<char*>(&nextReadSize), sizeof(unsigned int));
        if (stream.fail()) {
            std::cout << "FAILED" << std::endl;
            std::cerr << "Extracting size of object failed" << std::endl;
            break;
        }
        std::cout << "SUCCESS" << std::endl;

        std::cout << "\tNow extracting " << nextReadSize << " bytes . . . ";
        memset(&buffer[0], 0, sizeof(buffer));
        stream.read(reinterpret_cast<char*>(buffer), nextReadSize);
        if (stream.fail()) {
            std::cout << "FAILED" << std::endl;
            std::cerr << "Extracting object failed" << std::endl;
            break;
        }
        std::cout << "SUCCESS" << std::endl;

        std::cout << "\tCreating substream of size " << nextReadSize << " . . . ";
        subStream.clear();
        subStream.write(reinterpret_cast<const char*>(&buffer[0]), nextReadSize);
        std::cout << "SUCCESS" << std::endl;

        std::cout << "\tAppending " << objType << " to deck . . . " << std::endl;
        returnDeck.cards.emplace_front(std::move(Card::deserialise(subStream)));
    } while (!stream.eof());

    free(buffer);
    return std::make_unique<Deck>(std::move(returnDeck));
}

void Deck::add_card_front(std::unique_ptr<Card> card) {
    this->cards.emplace_front(std::move(card));
}

void Deck::add_card_back(std::unique_ptr<Card> card) {
    this->cards.emplace_back(std::move(card));
}

std::unique_ptr<Card> Deck::remove_card_back() {
    std::unique_ptr<Card> rtn(this->cards.back().release());
    this->cards.pop_back();
    return std::unique_ptr<Card>(rtn.release());
}

std::unique_ptr<Card> Deck::remove_card_front() {
    std::unique_ptr<Card> rtn(this->cards.front().release());
    this->cards.pop_front();
    return std::unique_ptr<Card>(rtn.release());
}

/**
 * Randomises the order of elements in the deque
 */
void Deck::shuffle() {
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(this->cards.begin(), this->cards.end(), std::default_random_engine(seed));
}

int Deck::size() {
    return this->cards.size();
}
