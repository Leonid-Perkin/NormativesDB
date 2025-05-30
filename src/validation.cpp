#include "validation.h"
#include <regex>
#include <limits>
#include <iostream>
#include <cctype>

bool isValidName(const std::string& str) {
    if (str.empty() || str.length() > 100) {
        return false;
    }
    for (const auto& c : str) {
        if (std::iscntrl(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

bool isValidGroupName(const std::string& str) {
    if (str.empty() || str.length() > 100) {
        return false;
    }
    for (const auto& c : str) {
        if (std::iscntrl(static_cast<unsigned char>(c))) {
            return false;
        }
        if (!(std::isdigit(static_cast<unsigned char>(c)) || c == ' ' || c == '-' || c == '.' || c >= 128)) {
            if (std::isalpha(static_cast<unsigned char>(c))) {
                return false;
            }
        }
    }
    return true;
}

bool isValidDate(const std::string& date) {
    std::regex datePattern(R"(\d{2}\.\d{2}\.\d{4})");
    return std::regex_match(date, datePattern);
}

bool isValidYear(int year) {
    return year >= 2000 && year <= 2025;
}

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}