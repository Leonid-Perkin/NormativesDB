#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <vector>
#include "student.h"

void saveToFile(const std::vector<Student>& students);
void loadFromFile(std::vector<Student>& students);

#endif