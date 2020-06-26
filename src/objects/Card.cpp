/**
 * @author Caleb Aitken
 * @date 20/06/2020
 *
 * Card object source
 */

#include "objects/Card.h"

void Card::serialise(std::ostream &stream) const {
    if (!stream.good()) {
        std::cerr << "ostream.good() failed" << std::endl;
    }

    std::string className = "Card";
    unsigned int classNameSize, nameSize, objSize;
    classNameSize = className.size();
    nameSize = this->name.size();
    objSize = (sizeof(int) * 4) + (nameSize + sizeof(unsigned int));

    std::cout << "Serialising Card . . . ";
    // header
    stream.write(reinterpret_cast<const char*>(&classNameSize), sizeof(unsigned int));
    stream.write(reinterpret_cast<const char*>(className.c_str()), classNameSize);
    stream.write(reinterpret_cast<const char*>(&objSize), sizeof(unsigned int));
    // data
    stream.write(reinterpret_cast<const char*>(&nameSize), sizeof(unsigned int));
    stream.write(reinterpret_cast<const char*>(this->name.data()), nameSize);
    stream.write(reinterpret_cast<const char*>(&this->baseAttack), sizeof(int));
    stream.write(reinterpret_cast<const char*>(&this->attackMods), sizeof(int));
    stream.write(reinterpret_cast<const char*>(&this->baseDefence), sizeof(int));
    stream.write(reinterpret_cast<const char*>(&this->defenceMods), sizeof(int));
    std::cout << "DONE" << std::endl;
}

std::unique_ptr<Card> Card::deserialise(std::istream &stream) {
    if (!stream.good()) {
        std::cerr << "istream.good() failed" << std::endl;
    }

    std::unique_ptr<Card> returnCard;

    unsigned int nextReadSize;
    char* buffer = (char*) calloc(1024, sizeof(char));
    std::string name;
    int* extractedInt = (int*) calloc(1, sizeof(int));

    std::cout << "\tExtracting size of name . . . ";
    stream.read(reinterpret_cast<char*>(&nextReadSize), sizeof(unsigned int));
    if (!stream.good()) {
        std::cout << "FAILED" << std::endl;
        std::cerr << "Extracting size of name failed" << std::endl;
        return nullptr;
    }
    std::cout << "SUCCESS" << std::endl;

    std::cout << "\tNow extracting " << nextReadSize << " bytes . . . ";
    stream.read(reinterpret_cast<char*>(buffer), nextReadSize);
    if (!stream.good()) {
        std::cout << "FAILED" << std::endl;
        std::cerr << "Extracting name failed" << std::endl;
        return nullptr;
    }
    std::cout << "SUCCESS" << std::endl;

    buffer[nextReadSize] = '\0';
    name.assign(buffer);

    if (name.length() != 2) {
        std::cerr << "Extracted name has incorrect parameters" << std::endl;
        return nullptr;
    } else {
        //returnCard->name[0] = name[0]; // FIXME: this causes a segfault
        //returnCard->name[1] = name[1];
    }

    // FIXME: cannot read the int from the stream
    std::cout << "\tNow extracting " << sizeof(int) << " bytes . . . ";
    stream.read(reinterpret_cast<char*>(extractedInt), sizeof(int));
    if (!stream.good()) {
        std::cout << "FAILED" << std::endl;
        std::cerr << "Extracting base attack failed" << std::endl;
        return nullptr;
    }
    std::cout << "SUCCESS" << std::endl;

    returnCard->baseAttack = *extractedInt;

    std::cout << "\tNow extracting " << sizeof(int) << " bytes . . . ";
    stream.read(reinterpret_cast<char*>(extractedInt), sizeof(int));
    if (!stream.good()) {
        std::cout << "FAILED" << std::endl;
        std::cerr << "Extracting attack mods failed" << std::endl;
        return nullptr;
    }
    std::cout << "SUCCESS" << std::endl;

    returnCard->attackMods = *extractedInt;

    std::cout << "\tNow extracting " << sizeof(int) << " bytes . . . ";
    stream.read(reinterpret_cast<char*>(extractedInt), sizeof(int));
    if (!stream.good()) {
        std::cout << "FAILED" << std::endl;
        std::cerr << "Extracting base defence failed" << std::endl;
        return nullptr;
    }
    std::cout << "SUCCESS" << std::endl;

    returnCard->baseDefence = *extractedInt;

    std::cout << "\tNow extracting " << sizeof(int) << " bytes . . . ";
    stream.read(reinterpret_cast<char*>(extractedInt), sizeof(int));
    if (!stream.good()) {
        std::cout << "FAILED" << std::endl;
        std::cerr << "Extracting defence mods failed" << std::endl;
        return nullptr;
    }
    std::cout << "SUCCESS" << std::endl;

    returnCard->defenceMods = *extractedInt;

    free(buffer);
    free(extractedInt);
    return std::unique_ptr<Card>(returnCard.release());
}
