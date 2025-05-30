#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>

bool isValidName(const std::string& str);
bool isValidGroupName(const std::string& str);
bool isValidDate(const std::string& date);
bool isValidYear(int year);
void clearInputBuffer();

#endif