//
// Created by caleb on 7/11/2020.
//

#include "objects/Player.h"

void Player::serialise(std::ostream &stream) const {
    if (!stream.good()) {
        std::cerr << "ostream.good() failed!" << std::endl;
    }

    const std::string className = "Player";
    unsigned int classNameSize, objSize;
    classNameSize = className.length();
    // Deck Header: int, char[4], int
    // Deck Data: Card[size]
        // Card Header: int, char[4], int
        // Card Data: int, char[2], int, int, int, int
    objSize = (2 * sizeof(unsigned int)) + 4;
    objSize += ((sizeof(int) * 4) + (2 + sizeof(unsigned int)) + (2 * sizeof(unsigned int)) + 4) * this->deck->size();

    std::cout << "Serialising Player . . . ";
    // header
    stream.write(reinterpret_cast<const char*>(&classNameSize), sizeof(unsigned int));
    stream.write(reinterpret_cast<const char*>(className.c_str()), classNameSize);
    stream.write(reinterpret_cast<const char*>(&objSize), sizeof(unsigned int));

    // data
    deck->serialise(stream);
}

std::unique_ptr<Player> Player::deserialise(std::istream &stream) {
    if (!stream.good()) {
        std::cerr << "istream.good() failed!" << std::endl;
    }

    Player returnPlayer;

    unsigned int nextReadSize = 0;
    char* buffer = (char*) calloc(1024, sizeof(char));
    std::string objType;
    std::stringstream subStream(std::ios::app | std::ios::out | std::ios::in | std::ios::binary);

    std::cout << "Now extracting next extract size . . .";
    stream.read(reinterpret_cast<char*>(&nextReadSize), sizeof(unsigned int));
    if (stream.eof()) {
        std::cout << "EOF" << std::endl << std::endl;
    } else if (stream.fail()) {
        std::cout << "FAILED" << std::endl << std::endl;
        std::cerr << "Extracting size of next read failed" << std::endl;
    }
    std::cout << "SUCCESS" << std::endl;

    std::cout << "\tNow extracting " << nextReadSize << " bytes . . . ";
    memset(&buffer[0], 0, sizeof(buffer));
    stream.read(reinterpret_cast<char*>(buffer), nextReadSize);
    if (stream.fail()) {
        std::cout << "FAILED" << std::endl;
        std::cerr << "Extracting next object type failed" << std::endl;
    }
    std::cout << "SUCCESS" << std::endl;

    objType.assign(buffer);

    std::cout << "\tNow extracting size of " << objType << " . . . ";
    stream.read(reinterpret_cast<char*>(&nextReadSize), sizeof(unsigned int));
    if (stream.fail()) {
        std::cout << "FAILED" << std::endl;
        std::cerr << "Extracting size of object failed" << std::endl;
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

    std::cout << "\tAppending " << objType << " to player . . . " << std::endl;

    returnPlayer.deck.reset(Deck::deserialise(subStream).release());

    free(buffer);
    return std::make_unique<Player>(std::move(returnPlayer));
}

std::shared_ptr<const Deck> Player::get_deck() {
    return this->deck;
}
