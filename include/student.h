#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <memory>
#include "normative.h"

struct Student {
    static void printTableHeader();
    void printTableRow() const;
    std::string surname;
    std::string initials;
    int admissionYear;
    std::string groupName;
    std::string recordBook;
    std::vector<std::shared_ptr<Normative>> tests;

    Student() = default;

    Student(const std::string& surname_, const std::string& initials_, int year_,
            const std::string& group_, const std::string& record_)
        : surname(surname_), initials(initials_), admissionYear(year_),
          groupName(group_), recordBook(record_) {}

    void print() const;
    double averageGrade(const std::string& normativeType = "") const;
    bool hasDebt(const std::string& normativeType) const;
};

#endif