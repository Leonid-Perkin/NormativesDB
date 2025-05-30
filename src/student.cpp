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
    std::cout << std::left
              << std::setw(15) << "Фамилия"
              << std::setw(10) << "Инициалы"
              << std::setw(10) << "Год"
              << std::setw(15) << "Группа"
              << std::setw(20) << "Зачетная книжка"
              << '\n';
    std::cout << std::string(70, '-') << '\n';
}

void Student::printTableRow() const {
    std::cout << std::left
              << std::setw(15) << surname
              << std::setw(10) << initials
              << std::setw(10) << admissionYear
              << std::setw(15) << groupName
              << std::setw(20) << recordBook
              << '\n';
}