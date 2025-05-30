#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include <vector>
#include <string>
#include "student.h"

void showMenu();
void addStudent(std::vector<Student>& students);
void deleteStudent(std::vector<Student>& students);
void editStudent(std::vector<Student>& students);
void printAll(const std::vector<Student>& students);
void printDebtorsByNorm(const std::vector<Student>& students);
void sortGroupsByAvgGrade(const std::vector<Student>& students);
void importFromTxt(std::vector<Student>& students);

#endif