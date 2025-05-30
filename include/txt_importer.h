#ifndef TXT_IMPORTER_H
#define TXT_IMPORTER_H

#include <vector>
#include <string>
#include "student.h"

void importStudentsFromTxt(const std::string& filename, std::vector<Student>& students);

#endif