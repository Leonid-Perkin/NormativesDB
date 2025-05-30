#include "student.h"
#include <iostream>
#include <iomanip>

void Student::print() const {
    std::cout << "Фамилия: " << surname
              << ", Инициалы: " << initials
              << ", Год поступления: " << admissionYear
              << ", Группа: " << groupName
              << ", Зачетная книжка: " << recordBook << "\n";
    std::cout << "Нормативы:" << std::endl;
    for (const auto& test : tests) {
        test->print();
    }
    std::cout << std::endl;
}

double Student::averageGrade(const std::string& normativeType) const {
    double sum = 0;
    int count = 0;
    for (const auto& test : tests) {
        if (normativeType.empty() || test->name == normativeType) {
            sum += test->grade;
            ++count;
        }
    }
    return count > 0 ? (sum / count) : 0.0;
}

bool Student::hasDebt(const std::string& normativeType) const {
    for (const auto& test : tests) {
        if ((normativeType.empty() || test->name == normativeType) && test->grade < 3) {
            return true;
        }
    }
    return false;
}


void Student::printTableHeader() {
    const int widthSurname = 15;
    const int widthInitials = 10;
    const int widthYear = 6;
    const int widthGroup = 12;
    const int widthRecordBook = 15;
    std::cout << std::left
              << std::setw(widthSurname) << "Фамилия" << " "
              << std::setw(widthInitials) << "Инициалы" << " "
              << std::setw(widthYear) << "Год" << " "
              << std::setw(widthGroup) << "Группа" << " "
              << std::setw(widthRecordBook) << "Зачетная книжка"
              << '\n';
    int totalWidth = widthSurname + widthInitials + widthYear + widthGroup + widthRecordBook + 4;
    std::cout << std::string(totalWidth, '-') << '\n';
}

void Student::printTableRow() const {
    std::string yearStr = std::to_string(admissionYear);
    const int widthSurname = 15;
    const int widthInitials = 10;
    const int widthYear = 6;
    const int widthGroup = 12;
    const int widthRecordBook = 15;
    std::cout << std::left
              << std::setw(widthSurname) << surname << " "
              << std::setw(widthInitials) << initials << " "
              << std::setw(widthYear) << yearStr << " "
              << std::setw(widthGroup) << groupName << " "
              << std::setw(widthRecordBook) << recordBook
              << '\n';
}