#include "txt_importer.h"
#include "validation.h"
#include "normative.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

void importStudentsFromTxt(const std::string& filename, std::vector<Student>& students) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }

    std::vector<Student> importedStudents;
    Student currentStudent;
    std::string line;
    bool inTestsSection = false;

    while (std::getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) {
            continue;
        }

        if (line == "--") {
            if (!currentStudent.surname.empty() || !currentStudent.recordBook.empty()) {
                importedStudents.push_back(std::move(currentStudent));
                currentStudent = Student();
            }
            inTestsSection = false;
            continue;
        }

        if (line == "Tests:") {
            inTestsSection = true;
            continue;
        }

        if (!inTestsSection) {
            size_t colonPos = line.find(':');
            if (colonPos == std::string::npos) {
                continue;
            }
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            if (key == "Surname") {
                if (!isValidName(value)) {
                    throw std::invalid_argument("Недопустимая фамилия: " + value);
                }
                currentStudent.surname = value;
            } else if (key == "Initials") {
                if (!isValidName(value)) {
                    throw std::invalid_argument("Недопустимые инициалы: " + value);
                }
                currentStudent.initials = value;
            } else if (key == "AdmissionYear") {
                try {
                    int year = std::stoi(value);
                    if (!isValidYear(year)) {
                        throw std::invalid_argument("Недопустимый год поступления: " + value);
                    }
                    currentStudent.admissionYear = year; // Fixed typo (was surname)
                } catch (...) {
                    throw std::invalid_argument("Недопустимый год поступления: " + value);
                }
            } else if (key == "Group") {
                if (!isValidGroupName(value)) {
                    throw std::invalid_argument("Недопустимое название группы: " + value);
                }
                currentStudent.groupName = value;
            } else if (key == "RecordBook") {
                if (value.length() > 20) {
                    throw std::invalid_argument("Слишком длинный номер зачетной книжки: " + value);
                }
                currentStudent.recordBook = value;
            }
        } else {
            std::stringstream ss(line);
            std::string name, date, resultStr, gradeStr;
            if (!std::getline(ss, name, ';') ||
                !std::getline(ss, date, ';') ||
                !std::getline(ss, resultStr, ';') ||
                !std::getline(ss, gradeStr, ';')) {
                throw std::invalid_argument("Неверный формат норматива: " + line);
            }

            name.erase(0, name.find_first_not_of(" \t"));
            name.erase(name.find_last_not_of(" \t") + 1);
            date.erase(0, date.find_first_not_of(" \t"));
            date.erase(date.find_last_not_of(" \t") + 1);
            resultStr.erase(0, resultStr.find_first_not_of(" \t"));
            resultStr.erase(resultStr.find_last_not_of(" \t") + 1);
            gradeStr.erase(0, gradeStr.find_first_not_of(" \t"));
            gradeStr.erase(gradeStr.find_last_not_of(" \t") + 1);

            if (!isValidDate(date)) {
                throw std::invalid_argument("Недопустимая дата норматива: " + date);
            }

            double result;
            try {
                result = std::stod(resultStr);
                if (result < 0) {
                    throw std::invalid_argument("Отрицательный результат норматива: " + resultStr);
                }
            } catch (...) {
                throw std::invalid_argument("Недопустимый результат норматива: " + resultStr);
            }

            int grade;
            try {
                grade = std::stoi(gradeStr);
                if (grade < 1 || grade > 5) {
                    throw std::invalid_argument("Недопустимая оценка норматива: " + gradeStr);
                }
            } catch (...) {
                throw std::invalid_argument("Недопустимая оценка норматива: " + gradeStr);
            }

            if (name == "Бег 100м") {
                currentStudent.tests.push_back(std::make_shared<Run100>(date, result, grade));
            } else if (name == "Бег 1000м") {
                currentStudent.tests.push_back(std::make_shared<Run1000>(date, result, grade));
            } else if (name == "Подтягивания") {
                currentStudent.tests.push_back(std::make_shared<PullUps>(date, result, grade));
            } else if (name == "Плавание") {
                currentStudent.tests.push_back(std::make_shared<Swim>(date, result, grade));
            } else {
                throw std::invalid_argument("Неизвестный норматив: " + name);
            }
        }
    }
    if (!currentStudent.surname.empty() || !currentStudent.recordBook.empty()) {
        importedStudents.push_back(std::move(currentStudent));
    }

    file.close();
    students.insert(students.end(),
                    std::make_move_iterator(importedStudents.begin()),
                    std::make_move_iterator(importedStudents.end()));
}