//
// Created by caleb on 13/04/2020.
//

#include "JsonTools.h"

json JsonTools::read_json_file(const std::string& jsonFile) {
    std::ifstream i(jsonFile);
    json j;
    i >> j;
    return j;
}