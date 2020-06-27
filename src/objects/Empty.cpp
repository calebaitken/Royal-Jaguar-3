/**
 * @author Caleb Aitken
 * @date 10/06/2020
 *
 * Empty
 */

#include "objects/Empty.h"

#include <memory>

void Empty::serialise(std::ostream& stream) const {
    if(!stream.good()) {
        std::cerr << "ostream.good() failed!" << std::endl;
    }

    std::string className = "Empty";
    unsigned int classNameSize = className.length(), objSize = 0;

    std::cout << "Serialising Empty . . . ";
    // header
    stream.write(reinterpret_cast<const char*>(&classNameSize), sizeof(unsigned int));
    stream.write(reinterpret_cast<const char*>(className.c_str()), classNameSize);
    stream.write(reinterpret_cast<const char*>(&objSize), sizeof(unsigned int));
    // data
    std::cout << "DONE" << std::endl;
}

std::unique_ptr<Empty> Empty::deserialise(std::istream& stream) {
    if(!stream.good()) {
        std::cerr << "istream.good() failed!" << std::endl;
    }

    std::cout << "\tCreating new object . . . SUCCESS" << std::endl;
    return std::make_unique<Empty>(Empty());
}
