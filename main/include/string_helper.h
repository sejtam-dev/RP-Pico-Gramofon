//
// Created by Tomáš Novák on 11.12.2022.
//

#ifndef GRAMOFON_STRING_HELPER_H
#define GRAMOFON_STRING_HELPER_H

#include <vector>
#include <string>

std::vector<std::string> splitString(const char* str, char seperator) {
    std::vector<std::string> result;

    do {
        const char *begin = str;
        while(*str != seperator && *str)
            str++;

        result.emplace_back(begin, str);
    } while (0 != *str++);

    return result;
}

#endif //GRAMOFON_STRING_HELPER_H
