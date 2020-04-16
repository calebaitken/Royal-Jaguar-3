//
// Created by caleb on 13/04/2020.
//

#ifndef ROYAL_JAGUAR_3_JSONTOOLS_H
#define ROYAL_JAGUAR_3_JSONTOOLS_H

#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace JsonTools {
    json read_json_file(const std::string& jsonFile);
}

#endif //ROYAL_JAGUAR_3_JSONTOOLS_H
