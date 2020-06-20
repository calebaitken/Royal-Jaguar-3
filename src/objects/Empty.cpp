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

    stream << "Empty:";
}

std::unique_ptr<Empty> Empty::deserialise(std::istream& stream) {
    if(!stream.good()) {
        std::cerr << "ostream.good() failed!" << std::endl;
    }

    return std::make_unique<Empty>(Empty());
}
